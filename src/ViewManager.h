#ifndef __VIEW_MANAGER__
#define __VIEW_MANAGER__

#include <string>

#include "ConsoleCanvas.h"
#include "MusicNote.h"

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
		void DrawNote(const idMusicNote &note, const int lane, const float laneLengthSeconds, const float time);
		void DrawBottomBar(bool* inputsHeld);
		void DrawUIBorder();
		std::string GetFormatedTime(const int time); // TODO: move to proper class
		void DrawUI(const std::string &songName, const int songLength);
		void UpdateUI(const int timeSinceStart, const int score, const int comboCount, const int missedNotes);
	private:
		idConsoleCanvas &canvas;
};

#endif