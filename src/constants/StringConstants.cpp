#include <sstream>

#include "GameConstants.h"
#include "InputConstants.h"
#include "StringConstants.h"

static std::string GetInstructions() {
	std::stringstream strStream;

	const std::string sectionSeparator("\n\n\n\n\n");

	// MAIN TITLE
	strStream << "WELCOME TO <UNTITLED RYTHM GAME>";
	strStream << sectionSeparator;

	// CHOOSING A SONG
	strStream << "CHOOSING A SONG\n\n\n";
	strStream << "Use '" << KeyConstants::AsString::MENU_NEXT << "' and '" << KeyConstants::AsString::MENU_PREVIOUS <<
	             "'\nto select a song.\n\n";
	strStream << "Then, press '" << KeyConstants::AsString::MENU_CONFIRM << "' to confirm.";
	strStream << sectionSeparator;

	// PLAYING THE GAME
	strStream << "PLAYING THE GAME\n\n\n";
	strStream << "To play the game, press the\n";
	for (int i = 0; i < GAME_LANE_COUNT; ++i) {
		strStream << "'" << KeyConstants::LANE_KEYS[i] << "' ";
	}
	strStream << "keys\nwith the correct timing.";
	strStream << sectionSeparator;

	// EXIT INSTRUCTIONS
	strStream << "To exit the game press '" << KeyConstants::AsString::APPLICATION_EXIT << "'.";

	return strStream.str();
}

namespace StringConstants {

	namespace LevelSelect {
		const std::string MAIN_TITLE = "SELECT A SONG";
		const std::string SELECTION_CURSOR = ">";
		const std::string HIGH_SCORE_TITLE = "HIGH SCORE";
		const std::string INSTRUCTIONS = GetInstructions();
	}

	namespace LevelPlay {
		const std::string SCORE_TITLE = "SCORE";
		const std::string COMBO_COUNT_TITLE = "COMBO";
		const std::string MISSED_NOTES_COUNT_TITLE = "MISS";
		const std::string ACCURACY_TITLE = "ACCURACY";
	}

	namespace LevelResults {
		const std::string ACCURACY_TITLE = "ACCURACY";
		const std::string MAX_COMBO_COUNT_TITLE = "MAX COMBO";
		const std::string PERFECT_COMBO_SUFFIX = " (PERFECT)";
		const std::string SCORE_TITLE = "SCORE";
		const std::string NEW_HIGH_SCORE_TITLE = "NEW HIGH SCORE";
		const std::string EXIT_SCREEN_TITLE =
			std::string("PRESS '") +
			KeyConstants::AsString::MENU_CONFIRM +
			std::string("' TO CONTINUE");
	}
}
