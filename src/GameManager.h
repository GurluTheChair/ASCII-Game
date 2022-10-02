#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include <string>

#include "GameLevel.h"
#include "InputManager.h"
#include "ViewManager.h"
#include "SoundManager.h"

class idGameManager {
	public:
		idGameManager(idInputManager& _input, idViewManager& _view, idSoundManager& _sound, const float _frameRate);

		bool InitGame(const std::string& levelFilename);
		void StartGame();
		void UpdateGame();

	private:
		idGameLevel currentLevel;
		idInputManager &input;
		idViewManager &view;
		idSoundManager &sound;
		float timeSinceStart;
		float deltaTime;
		const float frameRate;
		unsigned int comboCount;
		unsigned int missedNotes;
		unsigned int score;

		void UpdateGameData();
		void UpdateGameView();
};

#endif
