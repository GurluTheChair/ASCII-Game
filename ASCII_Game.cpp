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
	float position = 3;
	while (true) {
		view.Clear();
		view.DrawRectangle(idViewManager::rectangle_t{5,position,LANE_WIDTH,10}, 0x000F, 0x0000);
		view.Refresh();
		position += 0.15;
		Sleep(1);
	}
	return 0;
}
