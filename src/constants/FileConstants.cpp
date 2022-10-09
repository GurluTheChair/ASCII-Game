#include "FileConstants.h"

namespace PathConstants {
	const std::string RESOURCES_DIR = ".\\resources\\";

	namespace GameData {
		const std::string DIR = RESOURCES_DIR + "game_data\\";
		const std::string LEVELS_DIR = DIR + "songs\\";
		const std::string LEVEL_LIST = DIR + "songs_list.txt";
		const std::string LEVEL_HIGH_SCORES = DIR + "scores.txt";
	}

	namespace Audio {
		const std::string DIR = RESOURCES_DIR + "audio\\";
		const std::string SONGS_DIR = DIR + "songs\\";
		const std::string EFFECTS_DIR = DIR + "effects\\";

		namespace Effects {
			const std::string MENU_NAVIGATE = EFFECTS_DIR + "menu_navigate.wav";
			const std::string MENU_CONFIRM = EFFECTS_DIR + "menu_confirm.wav";
		}
	}
}