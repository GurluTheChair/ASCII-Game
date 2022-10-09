#include <windows.h>

#include "InputManager.h"
#include "ConsoleCanvas.h"
#include "ViewManager.h"
#include "SoundManager.h"
#include "GameManager.h"

int main(void) {
	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	idConsoleCanvas canvas(stdoutHandle);
	canvas.SetCursorVisible(false);

	idInputManager input;
	idViewManager view(canvas);
	idSoundManager sound;
	idGameManager game(input, view, sound, 60.0);

	return game.StartMainLoop();
}
