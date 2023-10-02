#include "HaloOnlineModules.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include "HaloOnline.hpp"
#include "Modules\ModuleGame.hpp"
#include "Modules\ModulePlayer.hpp"
#include "Modules\ModuleWeapon.hpp"
#include "Modules\ModuleServer.hpp"
#include "Modules\ModuleSettings.hpp"
#include "Modules\ModuleCamera.hpp"
#include "Modules\ModuleInput.hpp"
#include "Modules\ModuleTime.hpp"
#include "Modules\ModuleGraphics.hpp"
#include "Modules\ModuleUPnP.hpp"
#include "Modules\ModuleDebug.hpp"
#include "Modules\ModuleForge.hpp"
#include "Modules\ModuleVoIP.hpp"
#include "Modules\ModuleTweaks.hpp"
#include "Modules\ModuleCampaign.hpp"
#include <ShlObj.h>

namespace
{
	bool CommandHelp(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto& commandMap = Modules::CommandMap::Instance();
		if (Arguments.size() > 0)
		{
			auto cmdName = Arguments[0];
			auto* cmd = commandMap.FindCommand(cmdName);
			if (!cmd)
			{
				// try searching for it as a module
				bool isModule = false;
				for (auto it = commandMap.Commands.begin(); it < commandMap.Commands.end(); it++)
					if (it->ModuleName.length() > 0 && !_stricmp(it->ModuleName.c_str(), cmdName.c_str()))
					{
						isModule = true;
						break;
					}

				if (isModule)
					returnInfo = commandMap.GenerateHelpText(cmdName);
				else
					returnInfo = "Command/Variable not found";

				return isModule;
			}
			returnInfo = cmd->GenerateHelpText();
			return true;
		}
		returnInfo = commandMap.GenerateHelpText();
		return true;
	}

	bool CommandExecute(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		if (Arguments.size() <= 0)
		{
			returnInfo = "Usage: Execute <filename>";
			return false;
		}
		std::ifstream in(Arguments[0], std::ios::in | std::ios::binary);
		if (in && in.is_open())
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize((unsigned int)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			returnInfo = Modules::CommandMap::Instance().ExecuteCommands(contents, true);
			return true;
		}
		returnInfo = "Unable to open file " + Arguments[0] + " for reading.";
		return false;
	}

	bool CommandWriteConfig(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		PWSTR localAppdata;
		SHGetKnownFolderPath(FOLDERID_LocalAppData, NULL, NULL, &localAppdata);
		auto wide_haloonline_appdata = std::wstring(localAppdata);
		auto haloonline_appdata = Utils::String::ThinString(wide_haloonline_appdata);
		haloonline_appdata += "\\HaloOnline";

		std::string keyFile;
		std::string prefsName = "haloonline_prefs.cfg";
		if (HaloOnline::Instance().GetInstanceName() != "")
		{
			std::stringstream ss;
			ss << "haloonline_prefs_" << HaloOnline::Instance().GetInstanceName() << ".cfg";
			prefsName = ss.str();
			std::stringstream keystream;
			keystream << haloonline_appdata << "\\keys_" << HaloOnline::Instance().GetInstanceName() << ".cfg";
			keyFile = keystream.str();
		}
		else
		{
			std::stringstream ss;
			ss << haloonline_appdata << "\\keys.cfg";
			keyFile = ss.str();
		}
		if (Arguments.size() > 0)
			prefsName = Arguments[0];

		std::ofstream outFile(prefsName, std::ios::trunc);
		if (outFile.fail())
		{
			returnInfo = "Failed to write config to " + prefsName + "!";
			return false;
		}
		outFile << Modules::CommandMap::Instance().SaveVariables();
		outFile << "\n" << Modules::ModuleInput::Instance().ExportBindings();

		std::ofstream outKeys(keyFile, std::ios::trunc);
		if (outKeys.fail())
		{
			returnInfo += "\nFailed to write keys to " + keyFile;
			return false;
		}
		outKeys << Modules::CommandMap::Instance().SaveKeys();


		returnInfo = "Wrote config to " + prefsName + "\n" + "Wrote Key File to " + keyFile;
		return true;
	}
}

namespace Modules
{
	HaloOnlineModules::HaloOnlineModules() : ModuleBase("")
	{
		ModuleGame::Instance();
		ModulePlayer::Instance();
		ModuleWeapon::Instance();
		ModuleServer::Instance();
		ModuleSettings::Instance();
		ModuleCamera::Instance();
		ModuleInput::Instance();
		ModuleTime::Instance();
		ModuleGraphics::Instance();
		ModuleUPnP::Instance();
		ModuleDebug::Instance();
		ModuleForge::Instance();
		ModuleVoIP::Instance();
		ModuleTweaks::Instance();
		ModuleCampaign::Instance();

		AddCommand("Help", "help", "Displays this help text", eCommandFlagsNone, CommandHelp);
		AddCommand("Execute", "exec", "Executes a list of commands", eCommandFlagsNone, CommandExecute, { "filename(string) The list of commands to execute" });
		AddCommand("WriteConfig", "config_write", "Writes the Halo Online config file", eCommandFlagsNone, CommandWriteConfig, { "filename(string) Optional, the filename to write the config to" });

		CommandMap::Instance().FinishAddCommands(); // call this so that the default values can be applied to the game
	}
}
