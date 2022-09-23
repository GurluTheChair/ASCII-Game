#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

class idGameManager {
	public:
		idGameManager();
		bool InitGame(const std::string& levelFilename);
		void StartMainLoop();
		void UpdateGame();
	private:
		idGameLevel currentLevel;
		idInputManager input;
		idViewManager view;
		void UpdateGameData();
		void UpdateGameView();
};

#endif
