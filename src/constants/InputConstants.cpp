#include <windows.h>

#include "InputConstants.h"

namespace KeyConstants {
	const char LANE_KEYS[GAME_LANE_COUNT] = { 'S', 'D', 'F', 'G' };
	const char MENU_PREVIOUS = VK_UP;
	const char MENU_NEXT = VK_DOWN;
	const char MENU_CONFIRM = VK_RETURN;
	const char APPLICATION_EXIT = VK_ESCAPE;
	
	namespace AsString {
		const std::string MENU_PREVIOUS = "UP ARROW";
		const std::string MENU_NEXT = "DOWN ARROW";
		const std::string MENU_CONFIRM = "ENTER";
		const std::string APPLICATION_EXIT = "ESCAPE";
	}
}
