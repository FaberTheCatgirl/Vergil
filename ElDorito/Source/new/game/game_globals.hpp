#pragma once
#include "new\cseries\cseries.hpp"
#include "new\game\game_mode.hpp"

namespace Bungie
{
	struct game_globals
	{
		char unknown0[0x10];
		game_mode current_game_mode;
		char unknown14[0x13A];
		short current_insertion_point;
		char unknown150[0x250B8];
	};
	static_assert(sizeof(game_globals) == 0x25208);

	game_globals *__cdecl game_globals_get();

	inline bool game_globals_simulation_has_dist_server() { return ((bool(*)())0x531AF0)(); }
	
	inline void game_globals_primary_skull_toggle(__int16 skull, char enable) { ((void(*)(__int16, char))0x532B50)(skull, enable); }
	inline void game_globals_secondary_skull_toggle(__int16 skull, char enable) { ((void(*)(__int16, char))0x532EE0)(skull, enable); }

	inline bool __cdecl game_globals_primary_skull_is_active(__int16 skull) { return ((bool(__cdecl*)(__int16))0x60AE20)(skull); }
	inline bool __cdecl game_globals_secondary_skull_is_active(__int16 skull) { return ((bool(__cdecl*)(__int16))0x60AE50)(skull); }
}