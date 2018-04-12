#include "Input.h"

using namespace vs;

/// <summary>
/// Constructor
/// </summary>
input::input() {
	//Set all key states to none
	ZeroMemory(_ascii_keys, sizeof(_ascii_keys));
}

/// <summary>
/// Destructor
/// </summary>
input::~input() {
	
}

/// <summary>
/// Returns true if a key is pressed, false if not
/// </summary>
/// <param name="key">Key to check</param>
/// <returns></returns>
bool input::is_key_down(E_KEYS key) {
	const auto state = _ascii_keys[static_cast<int>(key)];
	return state >= pressed;
}

/// <summary>
/// Returns true, only if a key is held down
/// </summary>
/// <param name="key">Key to check</param>
/// <returns></returns>
bool input::is_key_held(E_KEYS key) {
	const auto state = _ascii_keys[static_cast<int>(key)];
	return state == held;
}

/// <summary>
/// Returns the instance of this class
/// </summary>
/// <returns></returns>
input& input::get_instance() {
	static input instance;
	return instance;
}

/// <summary>
/// Handles the given keyboard message
/// </summary>
/// <param name="keyboard_message"></param>
bool input::try_handle_keyboard_message(const MSG& keyboard_message) {
	auto was_handled = false;
	switch(keyboard_message.message) {
		case WM_KEYDOWN: {
			//Key was pressed
			const auto c = MapVirtualKey(keyboard_message.wParam, MAPVK_VK_TO_CHAR);
			const auto code = static_cast<int>(c);
			const auto current_state = static_cast<int>(_ascii_keys[code]);

			_ascii_keys[code] = current_state + 1 > static_cast<int>(held) ?
				held : static_cast<E_KEY_STATE>(current_state + 1);
			was_handled = true;
			break;
		}
		case WM_KEYUP: {
			//Key was released
			const auto c = MapVirtualKey(keyboard_message.wParam, MAPVK_VK_TO_CHAR);
			const auto code = static_cast<int>(c);

			_ascii_keys[code] = none;
			was_handled = true;
			break;
		}
	}
	return was_handled;
}


