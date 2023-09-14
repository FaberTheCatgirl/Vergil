#include "Patches\Events.hpp"
#include "Bungie\BlamEvents.hpp"
#include "Patch.hpp"
#include "Web\Ui\WebScoreboard.hpp"
#include "Web\Ui\ScreenLayer.hpp"

#include <vector>

using namespace Bungie::Events;
using namespace Patches::Events;

namespace
{
	void RunEventHook(Bungie::DatumHandle player, const Event *event, const EventDefinition *definition);
	std::vector<EventCallback> OnEventCallbacks;

}

namespace Patches::Events
{
	void ApplyAll()
	{
		Hook(0x16669C, RunEventHook, HookFlags::IsCall).Apply();
		Hook(0x165CE2, RunEventHook, HookFlags::IsCall).Apply();
		Hook(0x165D0D, RunEventHook, HookFlags::IsCall).Apply();
	}

	void OnEvent(EventCallback callback)
	{
		OnEventCallbacks.push_back(callback);
	}
}

namespace
{
	void RunEventHook(Bungie::DatumHandle player, const Event *event, const EventDefinition *definition)
	{
		// Dispatch the event to handlers
		for (auto &&callback : OnEventCallbacks)
			callback(player, event, definition);

		// Call the original function
		typedef void(*RunEventPtr)(Bungie::DatumHandle player, const Event *event, const EventDefinition *definition);
		auto RunEvent = reinterpret_cast<RunEventPtr>(0x5666B0);
		RunEvent(player, event, definition);
	}
}