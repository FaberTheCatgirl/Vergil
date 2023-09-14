#pragma once

#include <functional>

namespace Bungie::Input
{
	class VirtualKeyboard;
}

namespace Patches::VirtualKeyboard
{
	void ApplyAll();

	typedef std::function<void(Bungie::Input::VirtualKeyboard* keyboard)> KeyboardHandlerCallback;
	void SetKeyboardHandler(KeyboardHandlerCallback callback);
}

