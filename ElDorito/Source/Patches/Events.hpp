#pragma once
#include "Bungie\BlamData.hpp"
#include <functional>

namespace Bungie::Events
{
	struct Event;
	struct EventDefinition;
}

namespace Patches::Events
{
	void ApplyAll();

	// Callback for a Bungie event handler.
	typedef std::function<void(Bungie::DatumHandle player, const Bungie::Events::Event *event, const Bungie::Events::EventDefinition *definition)> EventCallback;

	// Registers a callback to be run when a Bungie event fires.
	void OnEvent(EventCallback callback);
}