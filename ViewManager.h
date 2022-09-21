#ifndef __VIEW_MANAGER__
#define __VIEW_MANAGER__

// Width of the screen
#define SCREEN_WIDTH 83
// Height of the screen
#define SCREEN_HEIGHT 37

// Number of characters per lane
#define LANE_WIDTH 7

// Color of the background
#define BACKGROUND_COLOR 0x0000
// Color of notes
#define NOTE_COLOR 0x0007
// Color of a note being pressed
#define PRESSED_COLOR 0x000F
// Color of a note that was missed
#define MISSED_COLOR 0x0004

class idViewManager {
	public:
		struct rectangle_t {
			int origin_x;
			float origin_y;
			int width;
			float height;
		};
		idViewManager(const HANDLE& outputHandle, const COORD& dwBufferSize, const COORD& dwBufferCoord, const SMALL_RECT& rcRegion);
		void HideCursor() const;
		CHAR_INFO GetCharInfo(const u_char displayValue, const WORD bgColor, const WORD fgColor) const;
		void DrawRectangle(const rectangle_t& rectangle, const WORD bgColor, const WORD fgColor);
		void Refresh() const;
	private:
		CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
		HANDLE outputHandle;
		COORD dwBufferSize;
		COORD dwBufferCoord;
		SMALL_RECT rcRegion;
};


#endif