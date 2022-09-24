#include <unordered_map>
#include <windows.h>

#include "InputManager.h"

void idInputManager::ResetKeyState(const int virtualKey) {
	short windowsKeyState = GetKeyState(virtualKey); // Windows-defined key state
	short keyState = keyStates[virtualKey]; // State defined by ourselves

	if (windowsKeyState & 0x8000) { // Key Being Pressed
		if (keyState & KEY_UP_BIT) {
			keyState |= KEY_PRESSED_BIT;
		}
		else {
			keyState &= ~KEY_PRESSED_BIT;
		}

		keyState |= KEY_HELD_BIT;
		keyState |= KEY_DOWN_BIT;
		keyState &= ~KEY_RELEASED_BIT;
		keyState &= ~KEY_UP_BIT;
	} else { // Key Not Being Pressed
		if (keyState & KEY_DOWN_BIT) {
			keyState |= KEY_RELEASED_BIT;
		} else {
			keyState &= ~KEY_RELEASED_BIT;
		}

		keyState |= KEY_UP_BIT;
		keyState &= ~KEY_DOWN_BIT;
		keyState &= ~KEY_HELD_BIT;
		keyState &= ~KEY_PRESSED_BIT;
	}

	keyStates[virtualKey] = keyState;
}

void idInputManager::RegisterKey(const int virtualKey) {
	keyStates[virtualKey] = 0;
	ResetKeyState(virtualKey);
}

void idInputManager::ResetKeyStates() {
	for (std::pair <const int, short> mapPair : keyStates) {
		ResetKeyState(mapPair.first);
	}
}

void idInputManager::UpdateKeyStates() {
	for (std::pair <const int, short> mapPair : keyStates) {
		short windowsKeyState = GetKeyState(mapPair.first); // Windows-defined key state
		short keyState = mapPair.second; // State defined by ourselves

		if (windowsKeyState & 0x8000) { // Key Being Pressed
			keyState |= KEY_DOWN_BIT;
			if (keyState & KEY_UP_BIT) {
				keyState |= KEY_PRESSED_BIT;
			}
			if (!(keyState & KEY_RELEASED_BIT)) { // We don't allow released keys to be "held"
				keyState |= KEY_HELD_BIT;
			}
			keyState &= ~KEY_UP_BIT;
		} else { // Key Not Being Pressed
			keyState |= KEY_UP_BIT;
			if (keyState & KEY_DOWN_BIT) {
				keyState |= KEY_RELEASED_BIT;
			}
			keyState &= ~KEY_DOWN_BIT;
			keyState &= ~KEY_HELD_BIT;
		}

		keyStates[mapPair.first] = keyState;
	}
}

bool idInputManager::WasKeyHeld(const int virtualKey) const {
	return keyStates.at(virtualKey) & KEY_HELD_BIT;
}

bool idInputManager::WasKeyReleased(const int virtualKey) const {
	return keyStates.at(virtualKey) & KEY_RELEASED_BIT;
}

bool idInputManager::WasKeyPressed(const int virtualKey) const {
	return keyStates.at(virtualKey) & KEY_PRESSED_BIT;
}
