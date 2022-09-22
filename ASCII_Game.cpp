#include <windows.h>

#include "ViewManager.h"

using namespace std;

COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
COORD dwBufferCoord = { 0, 0 };
SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

int main(void) {
	idViewManager::rectangle_t notes[5] = {
		{ 1 , -1, LANE_WIDTH ,5},
		{ 11, -7, LANE_WIDTH ,2},
		{ 21, -10, LANE_WIDTH ,1},
		{ 31, -13, LANE_WIDTH ,7},
		{ 11, -20, LANE_WIDTH ,20} 
	};

	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	idViewManager view(stdoutHandle, dwBufferSize, dwBufferCoord, rcRegion);
	view.HideCursor();
	
	while (true) {
		view.Clear();
		for (int i = 0; i < 5; i++) {
			WORD noteColor = (notes[i].origin_y > LANE_HEIGHT + 1) ? MISSED_COLOR : NOTE_COLOR;
			view.DrawRectangle(notes[i], BACKGROUND_COLOR, noteColor);
			notes[i].origin_y += 0.1f;
		}
		view.DrawBoard();
		view.Refresh();
		Sleep(1);
	}
	return 0;
}
