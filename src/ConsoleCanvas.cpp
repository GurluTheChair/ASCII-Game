#include <windows.h>

#include "ConsoleCanvas.h"

idConsoleCanvas::idConsoleCanvas(const HANDLE &_outputHandle) : outputHandle(_outputHandle) {
	SetConsoleScreenBufferSize(outputHandle, dwBufferSize);
	SetConsoleWindowInfo(outputHandle, true, &rcRegion);
	ReadConsoleOutput(outputHandle, (CHAR_INFO*)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void idConsoleCanvas::SetCursorVisible(const bool visible) {
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = visible;
	SetConsoleCursorInfo(outputHandle, &info);
}

void idConsoleCanvas::DrawSubpixelRectangle(const subpixelRectangle_t &rect, const WORD bgColor, const WORD fgColor) {
	int bottomY = int(floor(rect.origin_y));
	int topY = int(floor(rect.origin_y - rect.height));

	// Draw bottom of "subpixel" rectangle (if necessary)
	if (bottomY >= 0 && bottomY < SCREEN_HEIGHT) {
		int displayValue = lround((rect.origin_y - bottomY) * 7);
		CHAR_INFO bottomChar = GetCharInfoFromDisplayValue(displayValue, bgColor, fgColor);
		for (size_t i = 0; i < rect.width; i++) {
			buffer[bottomY][rect.origin_x + i] = bottomChar;
		}
	}

	// Draw middle of "subpixel" rectangle (if necessary)
	for (int i = min(bottomY - 1, SCREEN_HEIGHT - 1); ((i > topY) && (i >= 0)); i--) {
		CHAR_INFO middleChar = GetCharInfoFromDisplayValue(8, bgColor, fgColor);
		for (size_t j = rect.origin_x; j < rect.origin_x + rect.width; j++) {
			buffer[i][j] = middleChar;
		}
	}

	// Draw top of "subpixel" rectangle (if necessary)
	if (topY >= 0 && topY < SCREEN_HEIGHT) {
		int displayValue = lround((rect.origin_y - rect.height - topY) * 7) + 8;
		CHAR_INFO topChar = GetCharInfoFromDisplayValue(displayValue, bgColor, fgColor);
		for (size_t i = 0; i < rect.width; i++) {
			buffer[topY][rect.origin_x + i] = topChar;
		}
	}
}

void idConsoleCanvas::Refresh() {
	WriteConsoleOutput(outputHandle, (CHAR_INFO*)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

void idConsoleCanvas::DrawCharRectangle(const rectangle_t &rect, const WCHAR unicodeChar, const WORD bgColor, const WORD fgColor) {
	CHAR_INFO charToDraw = { unicodeChar, WORD(bgColor | fgColor) };

	for (int y = rect.origin_y; y < rect.origin_y + rect.height; ++y) {
		for (int x = rect.origin_x; x < rect.origin_x + rect.width; ++x) {
			buffer[y][x] = charToDraw;
		}
	}
}

void idConsoleCanvas::DrawCharVLine(const int x, const int startY, const int yLength, const WCHAR unicodeChar, const WORD bgColor, const WORD fgColor) {
	CHAR_INFO charToDraw = { unicodeChar, WORD(bgColor | fgColor) };

	for (int y = startY; y < startY + yLength; ++y) {
		buffer[y][x] = charToDraw;
	}
}

void idConsoleCanvas::DrawCharHLine(const int startX, const int xLength, const int y, const WCHAR unicodeChar, const WORD bgColor, const WORD fgColor) {
	CHAR_INFO charToDraw = { unicodeChar, WORD(bgColor | fgColor) };

	for (int x = startX; x < startX + xLength; ++x) {
		buffer[y][x] = charToDraw;
	}
}

void idConsoleCanvas::DrawString(const std::string &toDraw, const int x, const int y, const WORD bgColor, const WORD fgColor) {
	CHAR_INFO res;
	res.Attributes = bgColor | fgColor;
	for (size_t i = 0; (i < toDraw.length()) && (i < SCREEN_WIDTH); ++i) {
		res.Char.UnicodeChar = toDraw[i];
		buffer[y][x + i] = res;
	}
}

// Converts a display value (the "index" of a "subpixel" rectangle), to the right unicode character
// and also puts the right background and foreground color
CHAR_INFO idConsoleCanvas::GetCharInfoFromDisplayValue(const uint8_t displayValue, const WORD bgColor, const WORD fgColor) const {
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