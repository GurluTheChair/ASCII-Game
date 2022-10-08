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
	static const uint16_t BACKGROUND_COLOR = 0x0000; // Color of the background
	static const uint16_t TEXT_COLOR = 0x000F; // Color of text
	static const uint16_t NOTE_COLOR = 0x000F; // Color of active notes
	static const uint16_t PRESSED_COLOR = 0x000E; // Color of pressed notes
	static const uint16_t MISSED_COLOR = 0x0008; // Color of missed notes
	static const uint16_t LANE_COLORS_BASE[GAME_LANE_COUNT] = { 0x0001, 0x0002, 0x0004, 0x0006 };
	static const uint16_t LANE_COLORS_INTENSIFIED[GAME_LANE_COUNT] = { 0x0009, 0x000A, 0x000C, 0x000E };
}

#endif