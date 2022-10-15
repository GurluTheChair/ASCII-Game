#include <cmath>
#include <windows.h>

#include "constants/GameConstants.h"
#include "constants/ViewConstants.h"
#include "constants/StringConstants.h"
#include "constants/InputConstants.h"
#include "ViewManager.h"

using namespace ColorConstants;
using namespace StringConstants;

idViewManager::idViewManager(idConsoleCanvas &_canvas) : canvas(_canvas) { }

void idViewManager::ClearNotesArea() {
	idConsoleCanvas::rectangle_t rect;
	rect.originX = 0;
	rect.originY = 0;
	rect.width = NOTES_AREA_WIDTH;
	rect.height = CONSOLE_HEIGHT;

	canvas.DrawCharRectangle(rect, ' ', BACKGROUND_COLOR, BACKGROUND_COLOR);
}

void idViewManager::Refresh() {
	canvas.Refresh();
}

void idViewManager::DrawBottomBar(bool *inputsHeld, bool* hasError) {
	idConsoleCanvas::rectangle_t rect;
	rect.height = 1;
	rect.width = LANE_WIDTH;
	
	for (int i = 0; i < GAME_LANE_COUNT; i++) {
		rect.originX = i * LANE_WIDTH;
		rect.originY = CONSOLE_HEIGHT - 2;
		canvas.DrawCharRectangle(rect, 0x2584, 
			(hasError[i]) ? BAD_COLOR : TEXT_COLOR,
			(inputsHeld[i]) ? LANE_COLORS_BASE[i] : BACKGROUND_COLOR);
		rect.originY = CONSOLE_HEIGHT - 1;
		canvas.DrawCharRectangle(rect, KeyConstants::LANE_KEYS[i],
			(inputsHeld[i]) ? LANE_COLORS_BASE[i] : BACKGROUND_COLOR,
			(inputsHeld[i]) ? TEXT_COLOR : LANE_COLORS_BASE[i]);
	}
}

void idViewManager::DrawUIBorder() {
	const int UI_X_ORIGIN = CONSOLE_WIDTH - UI_WIDTH;

	// Draw horizontal lines
	const int hLineStartX = UI_X_ORIGIN + 1;
	const int hLineLength = UI_WIDTH - 2;
	canvas.DrawCharHLine(hLineStartX, hLineLength, 0, '=', BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawCharHLine(hLineStartX, hLineLength, 6, '=', BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawCharHLine(hLineStartX, hLineLength, CONSOLE_HEIGHT - 1, '=', BACKGROUND_COLOR, TEXT_COLOR);

	// Draw vertical lines
	const int leftVLinesX = UI_X_ORIGIN;
	const int rightVLinesX = UI_X_ORIGIN + UI_WIDTH - 1;
	canvas.DrawCharVLine(leftVLinesX, 1, 5, '|', BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawCharVLine(leftVLinesX, 7, CONSOLE_HEIGHT - 8, '|', BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawCharVLine(rightVLinesX, 1, 5, '|', BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawCharVLine(rightVLinesX, 7, CONSOLE_HEIGHT - 8, '|', BACKGROUND_COLOR, TEXT_COLOR);
}

std::string idViewManager::GetFormatedTime(const int time) {
	int minutes = time / 60;
	int seconds = time % 60;
	std::string res = "";
	if (minutes < 10) {
		res += "0";
	}
	res += std::to_string(minutes) + ":";
	if (seconds < 10) {
		res += "0";
	}
	return  res + std::to_string(seconds);
}

void idViewManager::DrawNote(const idMusicNote &note, const float laneLengthSeconds, const float time) {
	// Compute subpixel rectangle equivalent to note
	idConsoleCanvas::subpixelRectangle_t rect;
	const int LANE_HEIGHT = CONSOLE_HEIGHT - 2;

	rect.originX = note.column * LANE_WIDTH;
	rect.originY = LANE_HEIGHT * (1 + ((time - note.startSeconds) / laneLengthSeconds));
	rect.width = LANE_WIDTH;
	rect.height = LANE_HEIGHT * ((note.endSeconds - note.startSeconds) / laneLengthSeconds);

	// Compute color of note
	WORD noteColor = 0;
	switch (note.state) {
		case idMusicNote::state_t::ACTIVE:
			noteColor = NOTE_COLOR;
			break;
		case idMusicNote::state_t::PRESSED:
			noteColor = LANE_COLORS_INTENSIFIED[note.column];
			break;
		case idMusicNote::state_t::MISSED:
			noteColor = MISSED_COLOR;
			break;
		default:
			break;
	}

	canvas.DrawSubpixelRectangle(rect, BACKGROUND_COLOR, noteColor);
}

void idViewManager::DrawUI(const std::string &songName, const int songLength) {
	const int UI_X_ORIGIN = CONSOLE_WIDTH - UI_WIDTH;
	const std::string TIME_STRING = "00:00 / " + GetFormatedTime(songLength);
	const int INFO_STRING_X = UI_X_ORIGIN + (UI_WIDTH / 6);
	const int INFO_STRING_LENGTH = 9;

	// Draw the Infos
	canvas.DrawCenteredString(songName, UI_X_ORIGIN, 2, UI_WIDTH, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawCenteredString(TIME_STRING, UI_X_ORIGIN, 4, UI_WIDTH, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawStringN(LevelPlay::SCORE_TITLE, INFO_STRING_X, 8, INFO_STRING_LENGTH, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawStringN(LevelPlay::COMBO_COUNT_TITLE, INFO_STRING_X, 9, INFO_STRING_LENGTH, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawStringN(LevelPlay::MISSED_NOTES_COUNT_TITLE, INFO_STRING_X, 10, INFO_STRING_LENGTH, BACKGROUND_COLOR, TEXT_COLOR);
}

void idViewManager::UpdateUI(const int timeSinceStart, const int score, const int comboCount, const int missedNotes) {
	const int UI_X_ORIGIN = CONSOLE_WIDTH - UI_WIDTH;
	const int TIME_STRING_LENGTH = 13;
	const int INFO_DATA_X = UI_X_ORIGIN + (UI_WIDTH / 6) + 9;

	//Draw the Infos
	canvas.DrawString(GetFormatedTime(timeSinceStart), UI_X_ORIGIN + (UI_WIDTH / 2) - (TIME_STRING_LENGTH / 2), 4, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString(std::to_string(score), INFO_DATA_X, 8, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString(std::to_string(comboCount)+"     ", INFO_DATA_X, 9, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString(std::to_string(missedNotes), INFO_DATA_X, 10, BACKGROUND_COLOR, TEXT_COLOR);
}

void idViewManager::DrawSelectUI(const std::string* levelNames, const size_t size) {
	const int UI_X_ORIGIN = CONSOLE_WIDTH - UI_WIDTH;
	const int UI_LIST_ORIGIN_X = UI_X_ORIGIN + 4 + int(LevelSelect::SELECTION_CURSOR.length());
	const int UI_SCORE_TEXT_ORIGIN_Y = 8;
	const int UI_LIST_ORIGIN_Y = UI_SCORE_TEXT_ORIGIN_Y + 6;
	const int UI_EXIT_ORIGIN_Y = CONSOLE_HEIGHT - 5;

	canvas.DrawCenteredString(LevelSelect::MAIN_TITLE, UI_X_ORIGIN, 3, UI_WIDTH, BACKGROUND_COLOR, TEXT_COLOR);

	canvas.DrawCenteredString(LevelSelect::HIGH_SCORE_TITLE, UI_X_ORIGIN, UI_SCORE_TEXT_ORIGIN_Y, UI_WIDTH, BACKGROUND_COLOR, TEXT_COLOR);

	for (int i = 0; i < size; i++) {
		canvas.DrawString(levelNames[i], UI_LIST_ORIGIN_X, UI_LIST_ORIGIN_Y+i, BACKGROUND_COLOR, TEXT_COLOR);
	}

	canvas.DrawCenteredString(LevelSelect::INSTRUCTIONS, UI_X_ORIGIN, UI_EXIT_ORIGIN_Y, UI_WIDTH, BACKGROUND_COLOR, TEXT_COLOR);
}

void idViewManager::UpdateSelectUI(const size_t index, unsigned int highScore) {
	const int UI_X_ORIGIN = CONSOLE_WIDTH - UI_WIDTH;
	const int UI_ARROW_ORIGIN_X = UI_X_ORIGIN + 3;
	const int UI_SCORE_ORIGIN_Y = 10;
	const int UI_LIST_ORIGIN_Y = UI_SCORE_ORIGIN_Y +4;
	const int UI_CURSOR_ZONE_HEIGHT = CONSOLE_HEIGHT - 5 - UI_LIST_ORIGIN_Y;

	idConsoleCanvas::rectangle_t rect = { 
		UI_ARROW_ORIGIN_X, 
		UI_LIST_ORIGIN_Y, 
		int(LevelSelect::SELECTION_CURSOR.length()), 
		UI_CURSOR_ZONE_HEIGHT 
	};
	canvas.DrawCharRectangle(rect, ' ', BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString(LevelSelect::SELECTION_CURSOR, UI_ARROW_ORIGIN_X, int(UI_LIST_ORIGIN_Y + index), BACKGROUND_COLOR, TEXT_COLOR);

	canvas.DrawCharHLine(UI_X_ORIGIN+1, UI_WIDTH-2, UI_SCORE_ORIGIN_Y, ' ', BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawCenteredString(std::to_string(highScore), UI_X_ORIGIN, UI_SCORE_ORIGIN_Y, UI_WIDTH, BACKGROUND_COLOR, TEXT_COLOR);
}

void idViewManager::DrawConfirmedUI(const size_t index) {
	const int UI_X_ORIGIN = CONSOLE_WIDTH - UI_WIDTH;
	const int UI_LIST_ORIGIN_Y = 14;
	canvas.InvertLine(UI_X_ORIGIN + 1, int(UI_LIST_ORIGIN_Y + index), UI_WIDTH - 2);
}

void idViewManager::ClearUI() {
	const int UI_X_ORIGIN = CONSOLE_WIDTH - UI_WIDTH;
	const int TOP_WINDOW_HEIGHT = 5;

	idConsoleCanvas::rectangle_t rect;
	rect.originX = UI_X_ORIGIN+1;
	rect.originY = 1;
	rect.width = UI_WIDTH-2;
	rect.height = TOP_WINDOW_HEIGHT;
	canvas.DrawCharRectangle(rect, ' ', BACKGROUND_COLOR, TEXT_COLOR);

	rect.originY = 1 + TOP_WINDOW_HEIGHT + 1;
	rect.height = CONSOLE_HEIGHT - 3 - TOP_WINDOW_HEIGHT;
	canvas.DrawCharRectangle(rect, ' ', BACKGROUND_COLOR, TEXT_COLOR);
}

void idViewManager::ClearConsole() {
	canvas.ClearCanvas(BACKGROUND_COLOR, BACKGROUND_COLOR);
}