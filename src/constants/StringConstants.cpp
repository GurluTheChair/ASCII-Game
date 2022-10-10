#include "InputConstants.h"
#include "StringConstants.h"

namespace StringConstants {

	namespace LevelSelect {
		const std::string MAIN_TITLE = "SELECT A SONG";
		const std::string SELECTION_CURSOR = ">";
		const std::string HIGH_SCORE_TITLE = "HIGH SCORE";
		const std::string INSTRUCTIONS = // TODO : write actual instructions
			std::string("PRESS '") + 
			KeyConstants::AsString::APPLICATION_EXIT +
			std::string("' TO EXIT GAME");
	}

	namespace LevelPlay {
		const std::string SCORE_TITLE = "SCORE";
		const std::string COMBO_COUNT_TITLE = "COMBO";
		const std::string MISSED_NOTES_COUNT_TITLE = "MISS";
		const std::string ACCURACY_TITLE = "ACCURACY";
	}

	namespace LevelResults {
		const std::string SCORE_TITLE = "SCORE";
		const std::string HIGH_SCORE_TITLE = "HIGH_SCORE";
		const std::string NEW_HIGH_SCORE_TITLE = "HIGH_SCORE (NEW)";
		const std::string MAX_COMBO_COUNT_TITLE = "MAX COMBO";
		const std::string ACCURACY_TITLE = "ACCURACY";
		const std::string EXIT_SCREEN_TITLE =
			std::string("PRESS '") +
			KeyConstants::AsString::MENU_CONFIRM +
			std::string("' TO CONTINUE");
	}

}
