#include <windows.h>

#include "ViewManager.h"

using namespace std;

COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
COORD dwBufferCoord = { 0, 0 };
SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

int main(void) {
	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	idViewManager view(stdoutHandle, dwBufferSize, dwBufferCoord, rcRegion);

	view.HideCursor();
	while (true) {
		// TODO: draw rectangle to check whether code works
		// view.DrawRectangle();
		
		/*
		Fall();
		PrintGame();
		if (displayValues[0][5] == 0) {
			Spawn(5, 1);
			Spawn(15, 5);
		}
		Sleep(2);
		*/
	}
	return 0;
}
