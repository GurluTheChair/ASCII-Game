#ifndef __FILE_CONSTANTS__
#define __FILE_CONSTANTS__

#include <string>

namespace PathConstants {
	extern const std::string RESOURCES_DIR; // Directory path for project resources

	namespace GameData {
		extern const std::string DIR; // Directory path for game data
		extern const std::string LEVELS_DIR; // Directory path for levels
		extern const std::string LEVEL_LIST; // File path for level list
		extern const std::string LEVEL_HIGH_SCORES; // File path for high scores on levels
	}

	namespace Audio {
		extern const std::string DIR; // Directory path for audio
		extern const std::string SONGS_DIR; // Directory path for audio of songs
		extern const std::string EFFECTS_DIR; // Directory path for audio of effects

		namespace Effects {
			extern const std::string MENU_NAVIGATE; // File path for menu navigation sound effect
			extern const std::string MENU_CONFIRM; // File path for menu confirmation sound effect
			extern const std::string COMBO_BREAK; // File path for "breaking a combo" sound effect
		}
	}
}

#endif
