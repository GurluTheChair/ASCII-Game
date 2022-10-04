#ifndef __VIEW_MANAGER__
#define __VIEW_MANAGER__

#include <string>

#include "ConsoleCanvas.h"
#include "MusicNote.h"

// Width of the screen
#define SCREEN_WIDTH 97
// Height of the screen
#define SCREEN_HEIGHT 37

// Number of characters per lane
#define LANE_WIDTH 11
// Number of characters per lane
#define LANE_HEIGHT 35

// Width of the UI by number of characters
#define UI_WIDTH 50
// Number of characters that separates the UI from the game display
#define UI_SEPARATOR 3

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

		idViewManager(idConsoleCanvas &_canvas);
		
		void ClearNotesArea();
		void Refresh();
		uint16_t GetColorByLane(int lane);
		uint16_t GetPressedColorByLane(int lane);
		void DrawNote(const idMusicNote &note, const int lane, const float laneLengthSeconds, const float time);
		void DrawBottomBar(bool* inputsHeld);
		void DrawUIBorder(const uint16_t bgColor, const uint16_t fgColor);
		std::string GetFormatedTime(const int time); // TODO: move to proper class
		void DrawUI(const std::string &songName, const int songLength, const uint16_t bgColor, const uint16_t fgColor);
		void UpdateUI(const int timeSinceStart, const int score, const int comboCount, const int missedNotes, const uint16_t bgColor, const uint16_t fgColor);
	private:
		idConsoleCanvas &canvas;
};

#endif