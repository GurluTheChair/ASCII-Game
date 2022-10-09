#ifndef __CONSOLE_CANVAS__
#define __CONSOLE_CANVAS__

#include <wincontypes.h>
#include <string>

#include "constants/ViewConstants.h"

class idConsoleCanvas {
	public:
		struct subpixelRectangle_t {
			int origin_x;
			float origin_y;
			int width;
			float height;
		};

		struct rectangle_t {
			int origin_x, origin_y, width, height;
		};

		idConsoleCanvas(const HANDLE &_outputHandle);
		void SetCursorVisible(const bool visible);
		void DrawSubpixelRectangle(const subpixelRectangle_t &rectangle, const WORD bgColor, const WORD fgColor);
		void Refresh();
		void DrawCharRectangle(const rectangle_t &rectangle, const WCHAR unicodeChar, const WORD bgColor, const WORD fgColor);
		void DrawCharVLine(const int x, const int startY, const int yLength, const WCHAR unicodeChar, const WORD bgColor, const WORD fgColor);
		void DrawCharHLine(const int startX, const int xLength, const int y, const WCHAR unicodeChar, const WORD bgColor, const WORD fgColor);
		void DrawString(const std::string& toDraw, const int x, const int y, const WORD bgColor, const WORD fgColor);
		void DrawCenteredString(const std::string& toDraw, const int x, const int y, const int max_length, const WORD bgColor, const WORD fgColor);
		void InvertLine(const int x, const int y, const int max_length);
		void ClearCanvas(const WORD bgColor, const WORD fgColor);

	private:
		const HANDLE& outputHandle;
		const COORD dwBufferSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
		const COORD dwBufferCoord = { 0, 0 };
		SMALL_RECT rcRegion = { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 };
		CHAR_INFO buffer[CONSOLE_HEIGHT][CONSOLE_WIDTH];

		CHAR_INFO GetCharInfoFromDisplayValue(const uint8_t displayValue, const WORD bgColor, const WORD fgColor) const;

		idConsoleCanvas(const idConsoleCanvas& other) = delete;
		idConsoleCanvas& operator=(const idConsoleCanvas &other) = delete;
};

#endif