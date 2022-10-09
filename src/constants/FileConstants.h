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
	}
}

#endif
