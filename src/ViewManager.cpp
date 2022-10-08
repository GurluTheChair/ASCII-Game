#include <cmath>
#include <windows.h>

#include "constants/GameConstants.h"
#include "constants/ViewConstants.h"
#include "constants/InputConstants.h"
#include "ViewManager.h"

using namespace ColorConstants;

idViewManager::idViewManager(idConsoleCanvas &_canvas) : canvas(_canvas) { }

void idViewManager::ClearNotesArea() {
	idConsoleCanvas::rectangle_t rect;
	rect.origin_x = 0;
	rect.origin_y = 0;
	rect.width = NOTES_AREA_WIDTH;
	rect.height = CONSOLE_HEIGHT;

	canvas.DrawCharRectangle(rect, ' ', BACKGROUND_COLOR, BACKGROUND_COLOR);
}

void idViewManager::Refresh() {
	canvas.Refresh();
}

void idViewManager::DrawBottomBar(bool *inputsHeld) {
	idConsoleCanvas::rectangle_t rect;
	rect.height = 1;
	rect.width = LANE_WIDTH;
	
	for (int i = 0; i < GAME_LANE_COUNT; i++) {
		rect.origin_x = i * LANE_WIDTH;
		rect.origin_y = CONSOLE_HEIGHT - 2;
		canvas.DrawCharRectangle(rect, 0x2584, 0x00F0,
			(inputsHeld[i]) ? LANE_COLORS_BASE[i] : (BACKGROUND_COLOR << 4));
		rect.origin_y = CONSOLE_HEIGHT - 1;
		canvas.DrawCharRectangle(rect, KeyConstants::LANE_KEYS[i],
			(inputsHeld[i]) ? (LANE_COLORS_BASE[i] << 4) : (BACKGROUND_COLOR << 4),
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

void idViewManager::DrawNote(const idMusicNote &note, const int lane, const float laneLengthSeconds, const float time) {
	// Compute subpixel rectangle equivalent to note
	idConsoleCanvas::subpixelRectangle_t rect;
	const int LANE_HEIGHT = CONSOLE_HEIGHT - 2;

	rect.origin_x = lane * LANE_WIDTH;
	rect.origin_y = LANE_HEIGHT * (1 + ((time - note.startSeconds) / laneLengthSeconds));
	rect.width = LANE_WIDTH;
	rect.height = LANE_HEIGHT * ((note.endSeconds - note.startSeconds) / laneLengthSeconds);

	// Compute color of note
	WORD noteColor = 0;
	switch (note.state) {
		case idMusicNote::state_t::ACTIVE:
			noteColor = NOTE_COLOR;
			break;
		case idMusicNote::state_t::PRESSED:
			noteColor = LANE_COLORS_INTENSIFIED[lane];
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
	const std::string time_string = "00:00 / " + GetFormatedTime(songLength);
	const int time_string_length = 14;

	//Draw the Infos
	canvas.DrawString(songName, UI_X_ORIGIN + (UI_WIDTH / 2) - (int(songName.length()) / 2), 2, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString(time_string, UI_X_ORIGIN + (UI_WIDTH / 2) - (time_string_length / 2), 4, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString("SCORE    ", UI_X_ORIGIN + (UI_WIDTH / 6), 8, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString("COMBO    ", UI_X_ORIGIN + (UI_WIDTH / 6), 9, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString("MISS     ", UI_X_ORIGIN + (UI_WIDTH / 6), 10, BACKGROUND_COLOR, TEXT_COLOR);
}

void idViewManager::UpdateUI(const int timeSinceStart, const int score, const int comboCount, const int missedNotes) {
	const int UI_X_ORIGIN = CONSOLE_WIDTH - UI_WIDTH;
	const int time_string_length = 14;

	//Draw the Infos
	canvas.DrawString(GetFormatedTime(timeSinceStart), UI_X_ORIGIN + (UI_WIDTH / 2) - (time_string_length / 2), 4, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString(std::to_string(score)+"   ", UI_X_ORIGIN + (UI_WIDTH / 6) + 9, 8, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString(std::to_string(comboCount)+"   ", UI_X_ORIGIN + (UI_WIDTH / 6) + 9, 9, BACKGROUND_COLOR, TEXT_COLOR);
	canvas.DrawString(std::to_string(missedNotes), UI_X_ORIGIN + (UI_WIDTH / 6) + 9, 10, BACKGROUND_COLOR, TEXT_COLOR);
}