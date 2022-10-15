#include "ViewConstants.h"

namespace ColorConstants {
	const uint16_t BACKGROUND_COLOR = 0x0000;
	const uint16_t TEXT_COLOR = 0x000F;
	const uint16_t GOOD_COLOR = 0x000A;
	const uint16_t BAD_COLOR = 0x0004;
	const uint16_t NOTE_COLOR = 0x000F;
	const uint16_t PRESSED_COLOR = 0x000E;
	const uint16_t MISSED_COLOR = 0x0008;
	const uint16_t LANE_COLORS_BASE[GAME_LANE_COUNT] = { 0x0001, 0x0002, 0x0004, 0x0006 };
	const uint16_t LANE_COLORS_INTENSIFIED[GAME_LANE_COUNT] = { 0x0009, 0x000A, 0x000C, 0x000E };
}
