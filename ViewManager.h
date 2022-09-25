#ifndef __VIEW_MANAGER__
#define __VIEW_MANAGER__

// Width of the screen
#define SCREEN_WIDTH 50
// Height of the screen
#define SCREEN_HEIGHT 37

// Number of characters per lane
#define LANE_WIDTH 11
// Number of characters per lane
#define LANE_HEIGHT 35

// Color of the background
#define BACKGROUND_COLOR 0x0000
// Color of text
#define TEXT_COLOR 0x000F
// Color of notes
#define NOTE_COLOR 0x000F
// Color of a note being pressed
#define PRESSED_COLOR 0x000E
// Color of a note that was missed
#define MISSED_COLOR 0x0008

class idViewManager {
	public:
		struct rectangle_t {
			int origin_x;
			float origin_y;
			int width;
			float height;
		};

		idViewManager(const HANDLE& _outputHandle, const COORD& _dwBufferSize, const COORD& _dwBufferCoord, const SMALL_RECT& _rcRegion);
		void HideCursor() const;
		CHAR_INFO GetCharInfo(const u_char displayValue, const WORD bgColor, const WORD fgColor) const;
		void DrawRectangle(const rectangle_t& rectangle, const WORD bgColor, const WORD fgColor);
		void Clear();
		void Refresh();
		void DrawBoard();
		WORD GetColorByLane(int lane);
		WORD GetPressedColorByLane(int lane);
		void DrawCharRectangle(rectangle_t rect, const int unicodeChar, const WORD bgColor, const WORD fgColor);
		void DrawBottomBar(bool* inputsHeld);
		void DrawString(const std::string& toDraw, const int x, const int y, const WORD bgColor, const WORD fgColor);
		void DrawUI();
	private:
		CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
		HANDLE outputHandle;
		COORD dwBufferSize;
		COORD dwBufferCoord;
		SMALL_RECT rcRegion;
};


#endif