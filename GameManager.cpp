#include <istream>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <windows.h>

#include "MusicNote.h"
#include "GameLevel.h"
#include "InputManager.h"
#include "ViewManager.h"
#include "GameManager.h"

bool idGameManager::InitGame(const std::string& levelFilename) {
	currentLevel.LoadFile(levelFilename);

	return false; // TODO: replace with actual value
}

void idGameManager::StartMainLoop() {

}

void idGameManager::UpdateGame() {
	
}

void idGameManager::UpdateGameData() {
	// TODO : update data according to time
}

void idGameManager::UpdateGameView() {
	// TODO : display current state of the game
}
