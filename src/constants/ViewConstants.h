#ifndef __VIEW_CONSTANTS__
#define __VIEW_CONSTANTS__

#include "GameConstants.h"

// Width of a single note lane (in number of characters)
#define LANE_WIDTH 11

// Width of area with notes (in number of characters)
#define NOTES_AREA_WIDTH (LANE_WIDTH*GAME_LANE_COUNT)
// Width of the UI (in number of characters)
#define UI_WIDTH 50
// Number of characters that separates the UI from the notes area
#define UI_SEPARATOR 3

// Width of the console (in number of characters)
#define CONSOLE_WIDTH (NOTES_AREA_WIDTH+UI_SEPARATOR+UI_WIDTH)
// Height of the console (in number of characters)
#define CONSOLE_HEIGHT 37

typedef unsigned short uint16_t;
namespace ColorConstants {
	extern const uint16_t BACKGROUND_COLOR; // Color of the background
	extern const uint16_t TEXT_COLOR; // Color of text
	extern const uint16_t GOOD_COLOR; // Color of positive information
	extern const uint16_t BAD_COLOR; // Color of negative information
	extern const uint16_t NOTE_COLOR; // Color of active notes
	extern const uint16_t PRESSED_COLOR; // Color of pressed notes
	extern const uint16_t MISSED_COLOR; // Color of missed notes
	extern const uint16_t LANE_COLORS_BASE[GAME_LANE_COUNT]; // Base color for lanes
	extern const uint16_t LANE_COLORS_INTENSIFIED[GAME_LANE_COUNT]; // Intensified (brighter) color for lanes
}

#endif