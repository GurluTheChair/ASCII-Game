#include <windows.h>

#include "InputManager.h"
#include "ConsoleCanvas.h"
#include "ViewManager.h"
#include "SoundManager.h"
#include "GameManager.h"

#include "FileConstants.h"

int main(void) {
	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	idConsoleCanvas canvas(stdoutHandle);
	canvas.SetCursorVisible(false);

	idViewManager view(canvas);
	idInputManager input;
	idSoundManager sound;
	idGameManager game(input, view, sound, 60.0);

	if (game.InitGame(FileConstants::LEVELS_DIR_PATH + "mii_channel.txt")) {
		game.StartGame();
	} else {
		return -1;
	}

	return 0;
}
