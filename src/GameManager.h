#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

#include "NYTimer.h"
#include "GameLevel.h"
#include "InputManager.h"
#include "ViewManager.h"
#include "SoundManager.h"

class idGameManager {
	public:
		idGameManager(idInputManager& _input, idViewManager& _view, idSoundManager& _sound, const float _frameRate);
		void StartMainLoop();
	private:
		// Step of the game
		enum class gameStep_t { 
			LEVEL_SELECT, // Selecting a level to play
			LEVEL_PLAY, // Playing a level (a song)
			SCORE_SAVE, // Saving the new high score (if higher)
			QUIT // Quitting the application
		};

		int currentLevelId;
		idGameLevel currentLevel;
		idInputManager &input;
		idViewManager &view;
		idSoundManager &sound;

		std::vector<std::pair<std::string, std::string>> levelList;
		std::unordered_map<std::string, int> levelHighScores;
		int selectedLevelIndex;
		gameStep_t nextStep;

		NYTimer timer;
		float timeSinceStepStart;
		float deltaTime;
		const float frameRate;
		unsigned int comboCount;
		unsigned int missedNotes;
		unsigned int score;

		void PlayGameStep(std::function<bool(void)> stepInitFunc, std::function<bool(void)> stepUpdateFunc);
		bool LoadLevelsData();

		bool SelectLevelUpdate();
		
		bool PlayLevelInit();
		bool PlayLevelUpdate();
		// Separate update into two functions for easier code management
		void UpdateGameData();
		void UpdateGameView();
};

#endif
