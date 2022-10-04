#include <cmath>
#include <windows.h>

#include "ViewManager.h"

idViewManager::idViewManager(idConsoleCanvas& _canvas) : canvas(_canvas) { }

void idViewManager::ClearNotesArea() {
	idConsoleCanvas::rectangle_t rect;
	rect.origin_x = 0;
	rect.origin_y = 0;
	rect.width = LANE_WIDTH * 4;
	rect.height = SCREEN_HEIGHT;

	canvas.DrawCharRectangle(rect, ' ', BACKGROUND_COLOR, BACKGROUND_COLOR);
}

void idViewManager::Refresh() {
	canvas.Refresh();
}

WORD idViewManager::GetColorByLane(int lane) {
	const WORD laneColors[4] = {0x0001,0x0002,0x0004,0x0006};
	if (lane >= 0 && lane < 4) {
		return laneColors[lane];
	}
	return 0x0007;
}

WORD idViewManager::GetPressedColorByLane(int lane) {
	const WORD laneColorsPressed[4] = { 0x0009,0x000A,0x000C,0x000E };
	if (lane >= 0 && lane < 4) {
		return laneColorsPressed[lane];
	}
	return 0x000F;
}

void idViewManager::DrawBottomBar(bool* inputsHeld) {
	//REMPLACER 4 PAR GAME_LANE_COUNT
	idConsoleCanvas::rectangle_t rect;
	rect.height = 1;
	rect.width = LANE_WIDTH;
	
	char laneChars[4] = { 'A','Z','E','R' };
	for (int i = 0; i < 4; i++) {
		rect.origin_x = i * LANE_WIDTH;
		rect.origin_y = SCREEN_HEIGHT - 2;
		canvas.DrawCharRectangle(rect, 0x2584, 0x00F0,
			(inputsHeld[i]) ? GetColorByLane(i) : BACKGROUND_COLOR);
		rect.origin_y = SCREEN_HEIGHT - 1;
		canvas.DrawCharRectangle(rect, laneChars[i],
			(inputsHeld[i]) ? GetColorByLane(i) << 4 : BACKGROUND_COLOR,
			(inputsHeld[i]) ? TEXT_COLOR : GetColorByLane(i));
	}
}

void idViewManager::DrawUIBorder(const WORD bgColor, const WORD fgColor) {
	// TODO : REMPLACER 4 PAR GAME_LANE_COUNT
	const int UI_X_ORIGIN = LANE_WIDTH * 4 + UI_SEPARATOR;

	// Draw horizontal lines
	const int hLineStartX = UI_X_ORIGIN + 1;
	const int hLineLength = UI_WIDTH - 2;
	canvas.DrawCharHLine(hLineStartX, hLineLength, 0, '=', bgColor, fgColor);
	canvas.DrawCharHLine(hLineStartX, hLineLength, 6, '=', bgColor, fgColor);
	canvas.DrawCharHLine(hLineStartX, hLineLength, SCREEN_HEIGHT - 1, '=', bgColor, fgColor);

	// Draw vertical lines
	const int leftVLinesX = UI_X_ORIGIN;
	const int rightVLinesX = UI_X_ORIGIN + UI_WIDTH - 1;
	canvas.DrawCharVLine(leftVLinesX, 1, 5, '|', bgColor, fgColor);
	canvas.DrawCharVLine(leftVLinesX, 7, SCREEN_HEIGHT - 8, '|', bgColor, fgColor);
	canvas.DrawCharVLine(rightVLinesX, 1, 5, '|', bgColor, fgColor);
	canvas.DrawCharVLine(rightVLinesX, 7, SCREEN_HEIGHT - 8, '|', bgColor, fgColor);
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
			noteColor = GetPressedColorByLane(lane);
			break;
		case idMusicNote::state_t::MISSED:
			noteColor = MISSED_COLOR;
			break;
		default:
			break;
	}

	canvas.DrawSubpixelRectangle(rect, BACKGROUND_COLOR, noteColor);
}

void idViewManager::DrawUI(const std::string &songName, const int songLength, const WORD bgColor, const WORD fgColor) {
	int UI_X_ORIGIN = LANE_WIDTH * 4 + UI_SEPARATOR;
	const std::string time_string = "00:00 / " + GetFormatedTime(songLength);
	const int time_string_length = 14;

	//Draw the Infos
	canvas.DrawString(songName, UI_X_ORIGIN + (UI_WIDTH / 2) - (int(songName.length()) / 2), 2, bgColor, fgColor);
	canvas.DrawString(time_string, UI_X_ORIGIN + (UI_WIDTH / 2) - (time_string_length / 2), 4, bgColor, fgColor);
	canvas.DrawString("SCORE    ", UI_X_ORIGIN + (UI_WIDTH / 6), 8, bgColor, fgColor);
	canvas.DrawString("COMBO    ", UI_X_ORIGIN + (UI_WIDTH / 6), 9, bgColor, fgColor);
	canvas.DrawString("MISS     ", UI_X_ORIGIN + (UI_WIDTH / 6), 10, bgColor, fgColor);
}

void idViewManager::UpdateUI(const int timeSinceStart, const int score, const int comboCount, const int missedNotes, const WORD bgColor, const WORD fgColor) {
	int UI_X_ORIGIN = LANE_WIDTH * 4 + UI_SEPARATOR;
	const int time_string_length = 14;

	//Draw the Infos
	canvas.DrawString(GetFormatedTime(timeSinceStart), UI_X_ORIGIN + (UI_WIDTH / 2) - (time_string_length / 2), 4, bgColor, fgColor);
	canvas.DrawString(std::to_string(score)+"   ", UI_X_ORIGIN + (UI_WIDTH / 6) + 9, 8, bgColor, fgColor);
	canvas.DrawString(std::to_string(comboCount)+"   ", UI_X_ORIGIN + (UI_WIDTH / 6) + 9, 9, bgColor, fgColor);
	canvas.DrawString(std::to_string(missedNotes), UI_X_ORIGIN + (UI_WIDTH / 6) + 9, 10, bgColor, fgColor);
}