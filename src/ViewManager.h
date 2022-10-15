#ifndef __VIEW_MANAGER__
#define __VIEW_MANAGER__

#include <string>

#include "ConsoleCanvas.h"
#include "MusicNote.h"

class idViewManager {
	public:
		idViewManager(idConsoleCanvas &_canvas);
		
		void ClearNotesArea();
		void Refresh();
		void DrawNote(const idMusicNote &note, const float laneLengthSeconds, const float time);
		void DrawBottomBar(bool* inputsHeld, bool* hasError);
		void DrawUIBorder();
		std::string GetFormatedTime(const int time); // TODO: move to proper class
		void DrawUI(const std::string &songName, const int songLength);
		void UpdateUI(const int timeSinceStart, const int score, const int comboCount, const int missedNotes);
		void DrawSelectUI(const std::string* levelNames, const size_t size);
		void UpdateSelectUI(const size_t index, unsigned int highScore);
		void DrawConfirmedUI(const size_t index);
		void ClearUI();
		void ClearConsole();
	private:
		idConsoleCanvas &canvas;
};

#endif