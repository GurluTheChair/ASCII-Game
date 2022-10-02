#ifndef __INPUT_MANAGER__
#define __INPUT_MANAGER__

#include <unordered_map>

class idInputManager {
	public:
		// Whether key is currently down
		static const short KEY_DOWN_BIT = 0x0001;
		// Whether key is currently up
		static const short KEY_UP_BIT = 0x0002;
		// Whether key was held during all the calls (including the previous reset)
		static const short  KEY_HELD_BIT = 0x0004;
		// Whether the key was released (up after being down)
		static const short  KEY_RELEASED_BIT = 0x0008;
		// Whether the key was pressed (down after being up)
		static const short  KEY_PRESSED_BIT = 0x0010;

		idInputManager() = default;
		void ResetKeyStates();
		void UpdateKeyStates();
		void RegisterKey(const int virtualKey);
		bool WasKeyHeld(const int virtualKey) const;
		bool WasKeyReleased(const int virtualKey) const;
		bool WasKeyPressed(const int virtualKey) const;
	private:
		std::unordered_map<int, short> keyStates;
		void ResetKeyState(const int virtualKey);
		idInputManager(const idInputManager& other) = delete;
		idInputManager& operator=(const idInputManager& other) = delete;
};

#endif
