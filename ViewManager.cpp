#include <windows.h>

#include "ViewManager.h"

idViewManager::idViewManager(
	const HANDLE& outputHandle, 
	const COORD& dwBufferSize, 
	const COORD& dwBufferCoord, 
	const SMALL_RECT& rcRegion)
	: outputHandle(outputHandle)
	, dwBufferSize(dwBufferSize)
	, dwBufferCoord(dwBufferCoord)
	, rcRegion(rcRegion)
	, buffer()
{
	ReadConsoleOutput(outputHandle, (CHAR_INFO*)buffer, dwBufferSize, dwBufferCoord, rcRegion);

}

void idViewManager::HideCursor() const {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
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
	int bottomY = ((int)(rectangle.origin_y));
	int topY = ((int)(rectangle.origin_y - rectangle.height));

	int displayValue = (rectangle.origin_y - bottomY) * 8;
	CHAR_INFO bottomChar = GetCharInfo(displayValue, bgColor, fgColor);
	for (size_t i = 0; i < rectangle.width; i++) {
		buffer[bottomY][rectangle.origin_x+i] = bottomChar;
	}

	for (size_t i = bottomY-1; i > topY; i--) {
		CHAR_INFO fullBlock = GetCharInfo(8, bgColor, fgColor);
		for (size_t j = 0; j < rectangle.width; j++) {
			buffer[i][j] = fullBlock;
		}
	}

	displayValue = ((rectangle.origin_y - rectangle.height) - bottomY) * 8;
	CHAR_INFO topChar = GetCharInfo(displayValue, bgColor, fgColor);
	for (size_t i = 0; i < rectangle.width; i++) {
		buffer[topY][rectangle.origin_x+i] = topChar;
	}
}

void idViewManager::Refresh() const {
	WriteConsoleOutput(hOutput, (CHAR_INFO*)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}
