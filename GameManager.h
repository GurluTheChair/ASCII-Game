#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

class idGameManager {
	public:
		// TODO: make actual constructor for class
		idGameManager(idInputManager& _input, idViewManager& _view, const float _frameRate);
		bool InitGame(const std::string& levelFilename);
		void StartGame();
		void UpdateGame();
	private:
		idGameLevel currentLevel;
		idInputManager &input;
		idViewManager &view;
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
