#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <windows.h>

#include "MusicNote.h"
#include "GameLevel.h"
#include "InputManager.h"
#include "ViewManager.h"
#include "SoundManager.h"
#include "GameManager.h"

using namespace std;

COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
COORD dwBufferCoord = { 0, 0 };
SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

int main(void) {
	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	idViewManager view(stdoutHandle, dwBufferSize, dwBufferCoord, rcRegion);
	view.HideCursor();

	idInputManager input;
	idSoundManager sound;
	idGameManager game(input, view, sound, 60.0);

	if (game.InitGame(".\\songs\\mii_channel.txt")) {
		game.StartGame();
	} else {
		return -1;
	}

	return 0;
}
