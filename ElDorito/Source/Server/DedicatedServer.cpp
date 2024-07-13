#include <WS2tcpip.h>
#include "Server\DedicatedServer.hpp"
#include "Modules\ModuleBase.hpp"
#include "Blam\BlamNetwork.hpp"

namespace Server::DedicatedServer
{
	bool needToSetMode = false;

	void Init()
	{
		Blam::Network::SetLobbyType(Blam::eLobbyTypeMultiplayer);
		needToSetMode = true;
	}
	void Tick()
	{
		if (!needToSetMode) {
			if (Blam::Network::GetLobbyType() != Blam::eLobbyTypeMultiplayer) {
				Blam::Network::SetLobbyType(Blam::eLobbyTypeMultiplayer);
				needToSetMode = true;
			}
			// Faber: Code Written for Dynamic Gamemode Switching
			//else
			//
			//if (Blam::Network::GetLobbyType() != Blam::eLobbyTypeCampaign) {
			//	Blam::Network::SetLobbyType(Blam::eLobbyTypeCampaign);
			//	needToSetMode = true;
			//}
			//
			//else
			//
			//if (Blam::Network::GetLobbyType() != Blam::eLobbyTypeFireFight) {
			//	Blam::Network::SetLobbyType(Blam::eLobbyTypeFireFight);
			//	needToSetMode = true;
			//}

			return;
		}

		if (Blam::Network::GetLobbyType() == Blam::eLobbyTypeMultiplayer) {
			if (Blam::Network::SetNetworkMode(Blam::eNetworkModeSystemLink)) {
				needToSetMode = false;
			}
		}
	}
}