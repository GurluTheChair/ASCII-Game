#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include <string>
#include <functional>
#include <vector>

#include "NYTimer.h"
#include "GameLevel.h"
#include "InputManager.h"
#include "ViewManager.h"
#include "SoundManager.h"
#include "ScoreManager.h"

class idGameManager {
	public:
		idGameManager(idInputManager &_input, idViewManager &_view, idSoundManager &_sound, const float _frameRate);
		int StartMainLoop();
	private:
		// Step of the game
		enum class gameStep_t { 
			LEVEL_SELECT, // Selecting a level to play
			LEVEL_PLAY, // Playing a level (a song)
			LEVEL_RESULTS, // Display results for played level
			QUIT_SUCCESS, // Quitting the application (with success)
			QUIT_ERROR // Quitting the application (with error)
		};

		int currentLevelId;
		idGameLevel currentLevel;
		idInputManager &input;
		idViewManager &view;
		idSoundManager &sound;
		idScoreManager score;

		std::vector<std::pair<std::string, std::string>> levelList;
		size_t selectedLevelIndex;
		gameStep_t nextStep;

		NYTimer timer;
		float timeSinceStepStart;
		float deltaTime;
		const float frameRate;

		void PlayGameStep(std::function<bool(void)> stepInitFunc, std::function<bool(void)> stepUpdateFunc);
		bool LoadLevelsData();

		bool SelectLevelInit();
		bool SelectLevelUpdate();
		
		bool PlayLevelInit();
		bool PlayLevelUpdate();
		// Separate update into two functions for easier code management
		void UpdateGameData();
		void UpdateGameView();

		bool LevelResultsInit();
		bool LevelResultsUpdate();
};

#endif
