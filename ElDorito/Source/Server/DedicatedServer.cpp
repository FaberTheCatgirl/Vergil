#include <WS2tcpip.h>
#include "Server\DedicatedServer.hpp"
#include "Modules\ModuleBase.hpp"
#include "Bungie\BlamNetwork.hpp"

namespace Server::DedicatedServer
{
	bool needToSetMode = false;

	void Init()
	{
		Bungie::Network::SetLobbyType(Bungie::eLobbyTypeMultiplayer);
		needToSetMode = true;
	}
	void Tick()
	{
		if (!needToSetMode) {
			if (Bungie::Network::GetLobbyType() != Bungie::eLobbyTypeMultiplayer) {
				Bungie::Network::SetLobbyType(Bungie::eLobbyTypeMultiplayer);
				needToSetMode = true;
			}
			return;
		}

		if (Bungie::Network::GetLobbyType() == Bungie::eLobbyTypeMultiplayer) {
			if (Bungie::Network::SetNetworkMode(Bungie::eNetworkModeSystemLink)) {
				needToSetMode = false;
			}
		}
	}
}