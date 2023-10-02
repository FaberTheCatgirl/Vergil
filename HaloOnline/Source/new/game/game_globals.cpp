#include "HaloOnline.hpp"
#include "new\game\game_globals.hpp"

namespace Bungie
{
	game_globals *__cdecl game_globals_get()
	{
		return *(Bungie::game_globals **)HaloOnline::Instance().GetMainTls(0x3C);
	}
}