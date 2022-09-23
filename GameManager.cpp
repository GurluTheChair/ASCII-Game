#include <istream>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <windows.h>

#include "MusicNote.h"
#include "GameLevel.h"
#include "InputManager.h"
#include "GameManager.h"
#include "ViewManager.h"

bool idGameManager::InitGame(const std::string& levelFilename) {
	currentLevel.LoadFile(levelFilename);
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
