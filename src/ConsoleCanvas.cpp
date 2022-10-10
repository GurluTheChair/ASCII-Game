#include <windows.h>
#include <sstream>

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
	if (bottomY >= 0 && bottomY < CONSOLE_HEIGHT) {
		int displayValue = lround((rect.origin_y - bottomY) * 7);
		CHAR_INFO bottomChar = GetCharInfoFromDisplayValue(displayValue, bgColor, fgColor);
		for (size_t i = 0; i < rect.width; i++) {
			buffer[bottomY][rect.origin_x + i] = bottomChar;
		}
	}

	// Draw middle of "subpixel" rectangle (if necessary)
	for (int i = min(bottomY - 1, CONSOLE_HEIGHT - 1); ((i > topY) && (i >= 0)); i--) {
		CHAR_INFO middleChar = GetCharInfoFromDisplayValue(8, bgColor, fgColor);
		for (size_t j = rect.origin_x; j < rect.origin_x + rect.width; j++) {
			buffer[i][j] = middleChar;
		}
	}

	// Draw top of "subpixel" rectangle (if necessary)
	if (topY >= 0 && topY < CONSOLE_HEIGHT) {
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
	for (size_t i = 0; (i < toDraw.length()) && (i < CONSOLE_WIDTH); ++i) {
		res.Char.UnicodeChar = toDraw[i];
		buffer[y][x + i] = res;
	}
}

void idConsoleCanvas::DrawStringN(const std::string &toDraw, const int x, const int y, const int size, const WORD bgColor, const WORD fgColor) {
	CHAR_INFO res;
	res.Attributes = bgColor | fgColor;

	size_t toDrawLength = toDraw.length();
	for (size_t i = 0; (i < size) && (i < CONSOLE_WIDTH); ++i) {
		if (i < toDrawLength) {
			res.Char.UnicodeChar = toDraw[i];
		} else {
			res.Char.UnicodeChar = ' ';
		}
		buffer[y][x + i] = res;
	}
}

void idConsoleCanvas::DrawCenteredString(const std::string& toDraw, const int x, const int y, const int max_length, const WORD bgColor, const WORD fgColor) {
	if (toDraw.length() <= max_length) {
		int centered_x = x + (max_length / 2) - int(toDraw.length() / 2);
		DrawString(toDraw, centered_x, y, bgColor,fgColor);
	} else {
		DrawString(toDraw.substr(0, max_length), x, y, bgColor, fgColor);
	}
}

void idConsoleCanvas::DrawMultilineString(const std::string &toDraw, const int x, const int y, const WORD bgColor, const WORD fgColor, const bool centered, const int max_length) {
	std::stringstream multiline(toDraw);
	std::string line;

	int lineY = y;
	while (!(std::getline(multiline, line).fail())) {
		if (!centered) {
			DrawString(line, x, lineY, bgColor, fgColor);
		} else {
			DrawCenteredString(line, x, lineY, max_length, bgColor, fgColor);
		}
		lineY++;
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

void idConsoleCanvas::InvertLine(const int x, const int y, const int max_length) {
	int end = min(max_length, CONSOLE_WIDTH-x);
	for (size_t i = 0; i < end; ++i) {
		buffer[y][x + i].Attributes ^= COMMON_LVB_REVERSE_VIDEO;
	}
}

void idConsoleCanvas::ClearCanvas(const WORD bgColor, const WORD fgColor) {
	CHAR_INFO charToDraw = { ' ', WORD(bgColor | fgColor) };

	for (size_t i = 0; i < CONSOLE_HEIGHT; ++i) {
		for (size_t j = 0; j < CONSOLE_WIDTH; ++j) {
			buffer[i][j] = charToDraw;
		}
	}
}