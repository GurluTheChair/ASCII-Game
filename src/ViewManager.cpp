#include <windows.h>
#include <cmath>
#include <string>

#include "ViewManager.h"

idViewManager::idViewManager(
	const HANDLE& _outputHandle, 
	const COORD& _dwBufferSize, 
	const COORD& _dwBufferCoord, 
	const SMALL_RECT& _rcRegion)
: outputHandle(_outputHandle)
, dwBufferSize(_dwBufferSize)
, dwBufferCoord(_dwBufferCoord)
, rcRegion(_rcRegion) {
	SetConsoleScreenBufferSize(outputHandle, dwBufferSize);
	SetConsoleWindowInfo(outputHandle, true, &rcRegion);
	ReadConsoleOutput(outputHandle, (CHAR_INFO*)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void idViewManager::HideCursor() const {
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(outputHandle, &info);
}

CHAR_INFO idViewManager::GetCharInfo(const u_char displayValue, const WORD bgColor, const WORD fgColor) const {
	CHAR_INFO res;
	res.Attributes = bgColor | fgColor;
	if (displayValue > 0 && displayValue <= 8) {
		res.Attributes |= COMMON_LVB_REVERSE_VIDEO;
	}

	if (displayValue == 0 || displayValue == 8) {
		res.Char.UnicodeChar = ' ';
		return res;
	}

	res.Char.UnicodeChar = 0x2588 - displayValue;

	if (displayValue >= 8) {
		res.Char.UnicodeChar += 8;
	}
	return res;
}

void idViewManager::DrawRectangle(const idViewManager::rectangle_t& rectangle, const WORD bgColor, const WORD fgColor) {
	int bottomY = int(floor(rectangle.origin_y));
	int topY = int(floor(rectangle.origin_y - rectangle.height));

	if (bottomY >= 0 && bottomY < LANE_HEIGHT) {
		int displayValue = lround((rectangle.origin_y - bottomY) * 7);
		CHAR_INFO bottomChar = GetCharInfo(displayValue, bgColor, fgColor);
		for (size_t i = 0; i < rectangle.width; i++) {
			buffer[bottomY][rectangle.origin_x + i] = bottomChar;
		}
	}

	for (int i = min(bottomY - 1, LANE_HEIGHT - 1) ; ((i > topY) && (i >= 0)); i--) {
		CHAR_INFO fullBlock = GetCharInfo(8, bgColor, fgColor);
		for (size_t j = rectangle.origin_x; j < rectangle.origin_x + rectangle.width; j++) {
			buffer[i][j] = fullBlock;
		}
	}

	if (topY >= 0 && topY < LANE_HEIGHT) {
		int displayValue = lround((rectangle.origin_y - rectangle.height - topY) * 7) + 8;
		CHAR_INFO topChar = GetCharInfo(displayValue, bgColor, fgColor);
		for (size_t i = 0; i < rectangle.width; i++) {
			buffer[topY][rectangle.origin_x + i] = topChar;
		}
	}
}

void idViewManager::Clear() {
	for (size_t i = 0; i < SCREEN_HEIGHT; i++){
		for (size_t j = 0; j < SCREEN_WIDTH; j++){
			buffer[i][j].Char.UnicodeChar = ' ';
			buffer[i][j].Attributes = BACKGROUND_COLOR;
		}
	}
}

void idViewManager::Refresh() {
	WriteConsoleOutput(outputHandle, (CHAR_INFO*)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void idViewManager::DrawBoard() {
	CHAR_INFO line;
	line.Char.UnicodeChar = ' ';
	line.Attributes = 0x0010;

	for (size_t i = 0; i < SCREEN_WIDTH; i++) {
		buffer[SCREEN_HEIGHT -1][i] = line;
	}
}

WORD idViewManager::GetColorByLane(int lane) {
	WORD laneColors[4] = {0x0001,0x0002,0x0004,0x0006};
	if (lane >= 0 && lane < 4) {
		return laneColors[lane];
	}
	return 0x0007;
}

WORD idViewManager::GetPressedColorByLane(int lane) {
	WORD laneColorsPressed[4] = { 0x0009,0x000A,0x000C,0x000E };
	if (lane >= 0 && lane < 4) {
		return laneColorsPressed[lane];
	}
	return 0x000F;
}

void idViewManager::DrawCharRectangle(const rectangle_t rect, const int unicodeChar, const WORD bgColor, const WORD fgColor) {
	int originY = int(floor(rect.origin_y));
	int height = int(floor(rect.height));
	
	CHAR_INFO toDraw;
	toDraw.Attributes = bgColor | fgColor;
	toDraw.Char.UnicodeChar = unicodeChar;
	for (int i = originY; i < originY+height; i++) {
		for (int j = rect.origin_x; j < rect.origin_x+rect.width; j++) {
			buffer[i][j] = toDraw;
		}
	}
}

void idViewManager::DrawBottomBar(bool* inputsHeld) {
	//REMPLACER 4 PAR GAME_LANE_COUNT
	rectangle_t rect;
	rect.height = 1;
	rect.width = LANE_WIDTH;
	
	char laneChars[4] = { 'A','Z','E','R' };
	for (int i = 0; i < 4; i++) {
		rect.origin_x = i * LANE_WIDTH;
		rect.origin_y = SCREEN_HEIGHT - 2;
		DrawCharRectangle(rect, 0x2584, 0x00F0,
			(inputsHeld[i]) ? GetColorByLane(i) : BACKGROUND_COLOR);
		rect.origin_y = SCREEN_HEIGHT - 1;
		DrawCharRectangle(rect, laneChars[i],
			(inputsHeld[i]) ? GetColorByLane(i) << 4 : BACKGROUND_COLOR,
			(inputsHeld[i]) ? TEXT_COLOR : GetColorByLane(i));
	}
}

void idViewManager::DrawString(const std::string& toDraw, const int x, const int y, const WORD bgColor, const WORD fgColor) {
	CHAR_INFO res;
	res.Attributes = bgColor | fgColor;
	for (size_t i = 0; i < toDraw.length(); i++) {
		res.Char.UnicodeChar = toDraw[i];
		buffer[y][x + i] = res;
	}
}

void idViewManager::DrawUI() {

}
