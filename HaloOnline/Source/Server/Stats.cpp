#include <WS2tcpip.h>
#include <fstream>
#include "Server\Stats.hpp"
#include "Blam\BlamEvents.hpp"
#include "Blam\BlamNetwork.hpp"
#include "Patches\Events.hpp"
#include "Patches\Core.hpp"
#include "Modules\ModuleServer.hpp"
#include "Modules\ModulePlayer.hpp"
#include "Utils\Logger.hpp"
#include "HaloOnline.hpp"
#include "ThirdParty\rapidjson\writer.h"
#include "ThirdParty\HttpRequest.hpp"
#include "ThirdParty\rapidjson\document.h"
#include "Patches\Network.hpp"
#include <iomanip>

#include "new\game\game_globals.hpp"

namespace Server::Stats
{
	static const uint32_t playerInfoBase = 0x2162E08;

	//If we send stats right when the game ends, some of the team scores arent updated yet.
	//If we wait for the submit-stats lifecycle state to fire, some of the scores are already reset to 0.
	time_t sendStatsTime = 0;

	std::string playersInfoEndpoint;
	int numberOfRounds = 1;
	// retrieves master server endpoints from haloonline.json
	void GetStatsEndpoints(std::vector<std::string>& destVect)
	{
		std::ifstream in("mods/haloonline.json", std::ios::in | std::ios::binary);
		if (in && in.is_open())
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize((unsigned int)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();

			rapidjson::Document json;
			if (!json.Parse<0>(contents.c_str()).HasParseError() && json.IsObject())
			{
				if (json.HasMember("stats")) //
				{
					auto& statsObject = json["stats"];
					if (statsObject.HasMember("submitUrls"))
					{
						auto& submitUrls = statsObject["submitUrls"];

						for (rapidjson::SizeType i = 0; i < submitUrls.Size(); i++)
						{
							destVect.push_back(submitUrls[i].GetString());
						}
					}
				}
			}
		}
	}

	void GetPlayersInfoEndpoint()
	{
		std::ifstream in("mods/haloonline.json", std::ios::in | std::ios::binary);
		if (in && in.is_open())
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize((unsigned int)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();

			rapidjson::Document json;
			if (!json.Parse<0>(contents.c_str()).HasParseError() && json.IsObject())
			{
				if (json.HasMember("stats")) //
				{
					auto& statsObject = json["stats"];
					if (statsObject.HasMember("playerInfo"))
					{
						playersInfoEndpoint = statsObject["playerInfo"].GetString();
					}
				}
			}
		}
	}

	// Endpoint for getting information about players in the game. Data retrieved is set as a
	// variable that is synchronized to clients, and sent to the scoreboard (or any other screen layer) as json.
	DWORD WINAPI GetPlayersInfo_Thread(LPVOID lpParam)
	{
		if (playersInfoEndpoint.empty())
			return false;

		auto* session = Blam::Network::GetActiveSession();

		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("players");
		writer.StartArray();

		int peerIdx = session->MembershipInfo.FindFirstPeer();

		while (peerIdx != -1)
		{
			int playerIdx = session->MembershipInfo.GetPeerPlayer(peerIdx);

			if (playerIdx != -1)
			{
				auto* player = &session->MembershipInfo.PlayerSessions[playerIdx];
				uint16_t team = Pointer(playerInfoBase + (5696 * playerIdx) + 32).Read<uint16_t>();

				struct in_addr inAddr;
				inAddr.S_un.S_addr = session->GetPeerAddress(peerIdx).ToInAddr();
				
				char ipStr[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &inAddr, ipStr, sizeof(ipStr));

				char uid[17];
				Blam::Players::FormatUid(uid, player->Properties.Uid);

				writer.StartObject();
				writer.Key("name");
				writer.String(Utils::String::ThinString(player->Properties.ClientProperties.DisplayName).c_str());
				writer.Key("filteredName");
				writer.String(Utils::String::ThinString(player->Properties.DisplayName).c_str());
				writer.Key("serviceTag");
				writer.String(Utils::String::ThinString(player->Properties.ServiceTag).c_str());
				writer.Key("playerIndex");
				writer.Int(playerIdx);
				writer.Key("uid");
				writer.String(uid);
				writer.Key("ip");
				writer.String(ipStr);
				writer.EndObject();
			}

			peerIdx = session->MembershipInfo.FindNextPeer(peerIdx);
		}

		writer.EndArray();
		writer.EndObject();

		HttpRequest req(L"HaloOnline/" + Utils::String::WidenString(Utils::Version::GetVersionString()), L"", L"");

		try
		{
			// send the connection request
			std::string sendObject = s.GetString();
			if (!req.SendRequest(Utils::String::WidenString(playersInfoEndpoint), L"POST", L"", L"", L"Content-Type: application/json\r\n", (void*)sendObject.c_str(), sendObject.length()))
			{
				Utils::Logger::Instance().Log(Utils::LogTypes::Network, Utils::LogLevel::Info, "Unable to connect to player info endpoint");
			}

			// make sure the server replied with 200 OK
			std::wstring expected = L"HTTP/1.1 200 OK";
			if (req.responseHeader.length() < expected.length())
			{
				Utils::Logger::Instance().Log(Utils::LogTypes::Network, Utils::LogLevel::Info, "Invalid server query response.");
				return false;
			}

			// parse the json response
			std::string resp = std::string(req.responseBody.begin(), req.responseBody.end());
			rapidjson::Document json;
			
			if (json.Parse<0>(resp.c_str()).HasParseError() || !json.IsObject())
			{
				Utils::Logger::Instance().Log(Utils::LogTypes::Network, Utils::LogLevel::Info, "Invalid json returned from player info endpoint.");
				return false;
			}

			Modules::ModuleServer::Instance().VarPlayersInfo->ValueString = resp;
		}
		catch (...)
		{
			Utils::Logger::Instance().Log(Utils::LogTypes::Network, Utils::LogLevel::Info, "Exception while getting player info");
		}

		return true;
	}

	DWORD WINAPI CommandServerAnnounceStats_Thread(LPVOID lpParam)
	{
		auto* session = Blam::Network::GetActiveSession();
		if (Blam::Network::GetLobbyType() != Blam::eLobbyTypeMultiplayer || Blam::Network::GetNetworkMode() != Blam::eNetworkModeSystemLink)
			return false;

		std::vector<std::string> statsEndpoints;
		GetStatsEndpoints(statsEndpoints);

		if (statsEndpoints.size() == 0)
			return false;
		
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("gameVersion");
		writer.String(Utils::Version::GetVersionString().c_str());
		writer.Key("serverName");
		writer.String(Modules::ModuleServer::Instance().VarServerName->ValueString.c_str());
		writer.Key("serverPort");
		writer.Int(Modules::ModuleServer::Instance().VarServerPort->ValueInt);
		writer.Key("port");
		writer.Int(Pointer(0x1860454).Read<uint32_t>());
		writer.Key("hostPlayer");
		writer.String(Modules::ModulePlayer::Instance().VarPlayerName->ValueString.c_str());

		writer.Key("game");
		writer.StartObject();

		writer.Key("sprintEnabled");
		writer.Bool(Modules::ModuleServer::Instance().VarServerSprintEnabled->ValueInt != 0);
		writer.Key("sprintUnlimitedEnabled");
		writer.Bool(Modules::ModuleServer::Instance().VarServerSprintUnlimited->ValueInt != 0);
		writer.Key("maxPlayers");
		writer.Int(Modules::ModuleServer::Instance().VarServerMaxPlayers->ValueInt);

		std::string mapName((char*)Pointer(0x22AB018)(0x1A4));
		std::wstring mapVariantName((wchar_t*)Pointer(0x1863ACA));
		std::wstring variantName((wchar_t*)Pointer(0x23DAF4C));

		int32_t variantType = Pointer(0x023DAF18).Read<int32_t>();
		
		if ((Blam::game_globals_get()->current_game_mode == Blam::game_mode::mainmenu) && (mapName == "mainmenu"))
		{
			mapName = std::string((char*)Pointer(0x19A5E49));
			variantName = std::wstring((wchar_t*)Pointer(0x179254));
			variantType = Pointer(0x179250).Read<uint32_t>();
		}

		writer.Key("mapName");
		writer.String(Utils::String::ThinString(mapVariantName).c_str());
		writer.Key("mapFile");
		writer.String(mapName.c_str());
		writer.Key("variant");
		writer.String(Utils::String::ThinString(variantName).c_str());

		if (variantType >= 0 && variantType < Blam::eGameTypeCount)
		{
			writer.Key("variantType");
			writer.String(Blam::GameTypeNames[variantType].c_str());
		}

		uint32_t TeamMode = Pointer(0x019A6210).Read<uint32_t>();

		writer.Key("teamGame");
		writer.Bool(TeamMode != 0);
		
		if (TeamMode == 1)
		{
			writer.Key("teamScores");
			writer.StartArray();

			auto engineGlobalsPtr = HaloOnline::GetMainTls(0x48);

			if (engineGlobalsPtr)
			{
				auto engineGobals = engineGlobalsPtr[0](0x101F4);

				for (int t = 0; t < 8; t++)
				{
					auto teamScore = engineGobals(t * 0x1A).Read<Blam::TEAM_SCORE>();

					if (numberOfRounds > 1)
						writer.Int(teamScore.TotalScore);
					else
						writer.Int(teamScore.Score);
				}
			}

			writer.EndArray();
		}

		writer.EndObject();

		writer.Key("players");
		writer.StartArray();

		for (auto peerIdx = session->MembershipInfo.FindFirstPeer(); peerIdx != -1; peerIdx = session->MembershipInfo.FindNextPeer(peerIdx))
		{
			auto playerIdx = session->MembershipInfo.GetPeerPlayer(peerIdx);

			if (playerIdx == -1)
				continue;

			auto playerStats = Blam::Players::GetStats(playerIdx);
			auto* player = &session->MembershipInfo.PlayerSessions[playerIdx];

			std::stringstream color;
			color << "#" << std::setw(6) << std::setfill('0') << std::hex << player->Properties.Customization.Colors[Blam::Players::ColorIndices::Primary];

			struct in_addr inAddr;
			inAddr.S_un.S_addr = session->GetPeerAddress(peerIdx).ToInAddr();
			char ipStr[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &inAddr, ipStr, sizeof(ipStr));

			uint16_t team = Pointer(playerInfoBase + (5696 * playerIdx) + 32).Read<uint16_t>();

			char uid[17];
			Blam::Players::FormatUid(uid, player->Properties.Uid);

			Pointer pvpBase(0x23F5A98);

			#pragma region Player
			writer.StartObject();

			writer.Key("name");
			writer.String(Utils::String::ThinString(player->Properties.DisplayName).c_str());
			writer.Key("clientName");
			writer.String(Utils::String::ThinString(player->Properties.ClientProperties.DisplayName).c_str());
			writer.Key("serviceTag");
			writer.String(Utils::String::ThinString(player->Properties.ServiceTag).c_str());
			writer.Key("ip");
			writer.String(ipStr);
			writer.Key("team");
			writer.Int(team);
			writer.Key("playerIndex");
			writer.Int(playerIdx);
			writer.Key("uid");
			writer.String(uid);
			writer.Key("primaryColor");
			writer.String(color.str().c_str());
			writer.Key("playerGameStats");
			writer.StartObject();
			writer.Key("score");
			writer.Int(playerStats.Score);
			writer.Key("kills");
			writer.Int(playerStats.Kills);
			writer.Key("assists");
			writer.Int(playerStats.Assists);
			writer.Key("deaths");
			writer.Int(playerStats.Deaths);
			writer.Key("betrayals");
			writer.Int(playerStats.Betrayals);
			writer.Key("timeSpentAlive");
			writer.Int(playerStats.TimeSpentAlive);
			writer.Key("suicides");
			writer.Int(playerStats.Suicides);
			writer.Key("bestStreak");
			writer.Int(playerStats.BestStreak);

			writer.EndObject();
			#pragma endregion

			#pragma region Medals
			writer.Key("playerMedals");
			writer.StartArray();

			for (int i = 0; i < Blam::Tags::Objects::MedalType::MedalCount; i++)
			{
				if (playerStats.Medals[i] > 0)
				{
					writer.StartObject();
					writer.Key("medalName");
					writer.String(Blam::Tags::Objects::MedalTypeNames[i].c_str());
					writer.Key("count");
					writer.Int(playerStats.Medals[i]);
					writer.EndObject();
				}
			}

			writer.EndArray();
			#pragma endregion

			#pragma region Weapons
			writer.Key("playerWeapons");
			writer.StartArray();

			for (int i = 0; i < Blam::Tags::Objects::DamageReportingType::DamageCount; i++)
			{
				if (playerStats.WeaponStats[i].Initialized == 1)
				{
					writer.StartObject();

					writer.Key("weaponName");
					writer.String(Blam::Tags::Objects::DamageReportingTypeNames[i].c_str());
					writer.Key("weaponIndex");
					writer.Int(i + 1);
					writer.Key("kills");
					writer.Int(playerStats.WeaponStats[i].Kills);
					writer.Key("killedBy");
					writer.Int(playerStats.WeaponStats[i].KilledBy);
					writer.Key("betrayalsWith");
					writer.Int(playerStats.WeaponStats[i].BetrayalsWith);
					writer.Key("suicidesWith");
					writer.Int(playerStats.WeaponStats[i].SuicidesWith);
					writer.Key("headshotsWith");
					writer.Int(playerStats.WeaponStats[i].HeadshotsWith);

					writer.EndObject();
				}
			}

			writer.EndArray();
			#pragma endregion

			#pragma region Other
			writer.Key("otherStats");
			writer.StartObject();

			int nemesisIdx = 0;

			for (int i = 0, nemesisKills = 0; i < 16; i++)
			{
				uint16_t currKills = pvpBase((playerIdx * 0x40) + (i * 4) + 2).Read<uint16_t>();
				
				if (currKills <= nemesisKills)
					continue;

				nemesisKills = currKills;
				nemesisIdx = i;
			}

			writer.Key("nemesisIndex");
			writer.Int(nemesisIdx);

			writer.Key("kingsKilled");
			writer.Int(playerStats.KingsKilled);
			writer.Key("humansInfected");
			writer.Int(playerStats.HumansInfected);
			writer.Key("zombiesKilled");
			writer.Int(playerStats.ZombiesKilled);
			writer.Key("timeInHill");
			writer.Int(playerStats.TimeInHill);
			writer.Key("timeControllingHill");
			writer.Int(playerStats.TimeControllingHill);
			writer.EndObject();

			writer.Key("playerVersusPlayerKills");
			writer.StartArray();

			for (int i = 0; i < 16; i++)
				writer.Int(pvpBase((playerIdx * 0x40) + (i * 4)).Read<uint16_t>());

			writer.EndArray();
			writer.EndObject();
			#pragma endregion
		}

		writer.EndArray();
		writer.EndObject();

		for (auto server : statsEndpoints)
		{
			HttpRequest req(L"HaloOnline/" + Utils::String::WidenString(Utils::Version::GetVersionString()), L"", L"");

			try
			{
				std::string sendObject = s.GetString();

				if (!req.SendRequest(Utils::String::WidenString(server), L"POST", L"", L"", L"Content-Type: application/json\r\n", (void*)sendObject.c_str(), sendObject.length()))
					Utils::Logger::Instance().Log(Utils::LogTypes::Network, Utils::LogLevel::Info, "Unable to connect to stats server");
			}
			catch (...)
			{
				Utils::Logger::Instance().Log(Utils::LogTypes::Network, Utils::LogLevel::Info, "Exception while sending stats to server");
			}
		}		

		return true;
	}

	void LifeCycleStateChanged(Blam::LifeCycleState newState)
	{
		auto* session = Blam::Network::GetActiveSession();
		
		if (!session || !session->IsEstablished() || !session->IsHost())
			return;

		switch (newState)
		{
			case Blam::eLifeCycleStateStartGame:
			{
				auto thread = CreateThread(NULL, 0, GetPlayersInfo_Thread, (LPVOID)"", 0, NULL);
				break;
			}
		}
	}

	void OnGameStart()
	{
		auto* session = Blam::Network::GetActiveSession();
		if (!session || !session->IsEstablished() || !session->IsHost())
			return;

		auto get_number_of_rounds = (int(*)())(0x005504C0);
		numberOfRounds = get_number_of_rounds();
	}

	void OnEvent(Blam::DatumHandle player, const Blam::Events::Event *event, const Blam::Events::EventDefinition *definition)
	{
		auto* session = Blam::Network::GetActiveSession();

		if (!session || !session->IsEstablished() || !session->IsHost())
			return;

		switch (event->NameStringId)
		{
		case 0x4004D: // general_event_game_over
			time(&sendStatsTime);
			break;
		}
	}
}

namespace Server::Stats
{
	void Init()
	{
		Patches::Network::OnLifeCycleStateChanged(LifeCycleStateChanged);
		Patches::Events::OnEvent(OnEvent);
		Patches::Core::OnGameStart(OnGameStart);
		GetPlayersInfoEndpoint();
	}

	void NewPlayerJoined()
	{
		auto thread = CreateThread(NULL, 0, GetPlayersInfo_Thread, (LPVOID)"", 0, NULL);
	}

	void Tick()
	{
		auto* session = Blam::Network::GetActiveSession();

		if (!session || !session->IsEstablished() || !session->IsHost())
			return;

		time_t curTime1;
		time(&curTime1);

		if (sendStatsTime != 0)
		{
			// If we send stats right when the game ends, some of the team scores arent updated yet.
			// If we wait for the submit-stats lifecycle state to fire, some of the scores are already reset to 0.
			auto elapsed = curTime1 - sendStatsTime;

			if (elapsed > 1)
			{
				auto thread = CreateThread(NULL, 0, CommandServerAnnounceStats_Thread, (LPVOID)"", 0, NULL);
				sendStatsTime = 0;
			}
		}
	}
}