#ifndef __INPUT_MANAGER__
#define __INPUT_MANAGER__

// Whether key is currently down
#define KEY_DOWN_BIT 0x0001
// Whether key is currently up
#define KEY_UP_BIT 0x0002
// Whether key was held during all the calls (including the previous reset)
#define KEY_HELD_BIT 0x0004
// Whether the key was pressed (down then up)
#define KEY_RELEASED_BIT 0x0008

class idInputManager {
	public:
		void ResetKeyStates();
		void UpdateKeyStates();
		void RegisterKey(const int virtualKey);
		bool WasKeyReleased(const int virtualKey) const;
		bool WasKeyHeld(const int virtualKey) const;
		// short GetKeyCurrentState(const int virtualKey) const;
	private:
		std::unordered_map<int, short> keyStates;
		void ResetKeyState(const int virtualKey);
};

#endif
