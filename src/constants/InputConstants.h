#ifndef __INPUT_CONSTANTS__
#define __INPUT_CONSTANTS__

#include <string>

#include "GameConstants.h"

namespace KeyConstants {
	extern const char LANE_KEYS[GAME_LANE_COUNT];
	extern const char MENU_PREVIOUS;
	extern const char MENU_NEXT;
	extern const char MENU_CONFIRM;
	extern const char APPLICATION_EXIT;

	namespace AsString {
		extern const std::string MENU_PREVIOUS;
		extern const std::string MENU_NEXT;
		extern const std::string MENU_CONFIRM;
		extern const std::string APPLICATION_EXIT;
	}
}

#endif