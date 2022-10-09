#include <cmath>
#include <fstream>

#include "constants/GameConstants.h"
#include "constants/FileConstants.h"
#include "constants/InputConstants.h"
#include "NYTimer.h"
#include "MusicNote.h"
#include "GameManager.h"

idGameManager::idGameManager(idInputManager& _input, idViewManager& _view, idSoundManager& _sound, const float _frameRate)
: input(_input)
, view(_view)
, sound(_sound)
, frameRate(_frameRate)
, deltaTime(0.0f)
, timeSinceStepStart(0.0f)
, comboCount(0)
, score(0)
, missedNotes(0)
, currentLevelId(0)
, nextStep(gameStep_t::LEVEL_SELECT)
, levelList()
, levelHighScores()
, selectedLevelIndex(0) {
	// Register keys used in program
	for (int i = 0; i < GAME_LANE_COUNT; ++i) {
		input.RegisterKey(KeyConstants::LANE_KEYS[i]);
	}

	// Load data about levels
	if (!LoadLevelsData()) {
		nextStep = gameStep_t::QUIT;
	}
}

bool idGameManager::LoadLevelsData() {
	// Load level list
	std::ifstream file(PathConstants::GameData::LEVEL_LIST);
	if (!file.good() || !file.is_open()) {
		return false;
	}
	
	std::string levelFileName;
	std::string levelDisplayName;
	std::pair<std::string, std::string> levelListElement;
	while (!file.eof()) {
		file >> levelFileName;
		if (file.fail()) {
			break; // Fail at file name retrieval, we assume it's the end of file
		}
		file >> std::ws;
		std::getline(file, levelDisplayName);
		if (file.fail()) {
			return false; // Fail at display name retrieval, the file is invalid
		}
		levelListElement.first = levelFileName;
		levelListElement.second = levelDisplayName;
		levelList.push_back(levelListElement);
	}

	// Load high score list
	file.close();
	file.clear();
	file.open(PathConstants::GameData::LEVEL_HIGH_SCORES);
	if (!file.good() || !file.is_open()) {
		return true; // File does not exist, do nothing
	}
	if (file.peek() == std::ios::traits_type::eof()) {
		return true; // File is empty, do nothing
	}

	int levelHighScore = 0;
	while (!file.eof()) {
		file >> levelFileName;
		if (file.fail()) {
			return true; // Fail at file name retrieval, we assume it's the end of file
		}
		file >> levelHighScore;
		if (file.fail()) {
			return false; // Fail at high score retrieval, the file is invalid
		}
		levelHighScores[levelFileName] = levelHighScore;
	}

	return true;
}

void idGameManager::StartMainLoop() {
	std::function<bool(void)> stepInitFunc = NULL;
	std::function<bool(void)> stepUpdateFunc = NULL;

	while (nextStep != gameStep_t::QUIT) {
		// Setup Init and Update function according to next step
		switch (nextStep) {
			case gameStep_t::LEVEL_SELECT:
				stepInitFunc = NULL;
				stepUpdateFunc = std::bind(&idGameManager::SelectLevelUpdate, this);
				break;
			case gameStep_t::LEVEL_PLAY:
				stepInitFunc = std::bind(&idGameManager::PlayLevelInit, this);
				stepUpdateFunc = std::bind(&idGameManager::PlayLevelUpdate, this);
				break;
			case gameStep_t::SCORE_SAVE:
				// TODO : implement this
				// TODO : remove quit
				stepInitFunc = NULL;
				stepUpdateFunc = NULL;
				nextStep = gameStep_t::QUIT;
				break;
			default:
				stepInitFunc = NULL;
				stepUpdateFunc = NULL;
				nextStep = gameStep_t::QUIT;
				break;
		}
		// Play next game step
		PlayGameStep(stepInitFunc, stepUpdateFunc);
	}
}

void idGameManager::PlayGameStep(std::function<bool(void)> stepInitFunc, std::function<bool(void)> stepUpdateFunc) {
	// If no function given, skip step
	if ((stepInitFunc == NULL) && (stepUpdateFunc == NULL)) {
		return;
	}

	if (stepInitFunc != NULL) {
		// If init function failed, stop step
		if (!stepInitFunc()) {
			nextStep = gameStep_t::QUIT;
			return;
		}
	}

	// If no function given, skip step
	if (stepUpdateFunc == NULL) {
		return;
	}
	
	// Prepare Update Loop
	const float delayBetweenFrames = 1.0f / frameRate;
	timeSinceStepStart = 0.0f;
	bool shouldStop = stepUpdateFunc();

	float startTime = timer.getElapsedSeconds();
	float previousUpdateTime = startTime;
	float currentLoopTime;

	// Play Update Loop
	while (!shouldStop) {
		input.UpdateKeyStates();
		currentLoopTime = timer.getElapsedSeconds();

		if (currentLoopTime > (previousUpdateTime + delayBetweenFrames)) {
			timeSinceStepStart = currentLoopTime - startTime;
			deltaTime = currentLoopTime - previousUpdateTime;

			shouldStop = stepUpdateFunc();
			sound.UpdateSourceStates();
			input.ResetKeyStates();

			previousUpdateTime = currentLoopTime;
		}
		Sleep(1);
	}
}

bool idGameManager::SelectLevelUpdate() {
	// TODO : check up and down arrow keys

	// TODO : check enter key (and return true)

	// TODO : display UI
	selectedLevelIndex = 0;
	nextStep = gameStep_t::LEVEL_PLAY;

	return true;
}

bool idGameManager::PlayLevelInit() {
	// Load level
	std::string levelFilename = PathConstants::GameData::LEVELS_DIR;
	levelFilename.append(levelList[selectedLevelIndex].first);

	if (!currentLevel.LoadFile(levelFilename)) {
		return false;
	}
		

	// Load level music data and play it
	std::string songFilePath = PathConstants::Audio::SONGS_DIR;
	songFilePath.append(currentLevel.GetSongFilename());

	if (!sound.LoadWav(songFilePath)) {
		return false;
	}

	if (!sound.Play(songFilePath)) {
		return false;
	}

	// Prepare scoring properties
	comboCount = 0;
	missedNotes = 0;
	score = 0;

	// Draw UI
	const float songLength = currentLevel.GetLengthSeconds();
	view.DrawUIBorder();
	view.DrawUI(currentLevel.GetSongName(), int(songLength));

	return true;
}

bool idGameManager::PlayLevelUpdate() {
	UpdateGameData();
	UpdateGameView();

	if (timeSinceStepStart <= currentLevel.GetLengthSeconds()) {
		return false;
	} else {
		nextStep = gameStep_t::QUIT;
		return true;
	}
}

void idGameManager::UpdateGameData() {
	// # Input Management
	currentLevel.ActivateNotesForTime(timeSinceStepStart);

	const float pressEarlyTolerance = 0.1f;
	const float pressLateTolerance = 0.1f;
	const float releaseEarlyTolerance = 0.15f;
	const float maxMissTimeDistance = 0.1f;

	for (int i = 0; i < GAME_LANE_COUNT; ++i)
	{
		// Get active notes for current lane
		std::deque<idMusicNote> &laneNotes = currentLevel.GetEditableActiveNotes(i);
		size_t laneNotesCount = laneNotes.size();
		if (laneNotesCount <= 0) {
			continue;
		}

		// Retrieve the "lowest" (closest to the bottom) note that wasn't already played by the player
		int bottomNoteIndex = 0;
		idMusicNote* bottomNote;
		do {
			bottomNote = &laneNotes[bottomNoteIndex++];
		} while (
			(bottomNoteIndex < laneNotesCount) &&
			(bottomNote->state != idMusicNote::state_t::ACTIVE) && 
			(timeSinceStepStart > bottomNote->endSeconds - releaseEarlyTolerance));

		// Update note state 
		if (bottomNote->state != idMusicNote::state_t::MISSED) {
			if (bottomNote->state == idMusicNote::state_t::ACTIVE) {
				if (timeSinceStepStart > bottomNote->startSeconds + pressLateTolerance) {
					bottomNote->state = idMusicNote::state_t::MISSED;
				} else if (input.WasKeyPressed(KeyConstants::LANE_KEYS[i])) {
					if (timeSinceStepStart >= bottomNote->startSeconds - pressEarlyTolerance) {
						bottomNote->state = idMusicNote::state_t::PRESSED;
					}
					else if (timeSinceStepStart + maxMissTimeDistance >= bottomNote->startSeconds - pressEarlyTolerance) {
						bottomNote->state = idMusicNote::state_t::MISSED;
					}
				}
			} else if (bottomNote->state == idMusicNote::state_t::PRESSED) {
				if (input.WasKeyReleased(KeyConstants::LANE_KEYS[i]) &&
					(timeSinceStepStart <= bottomNote->endSeconds - releaseEarlyTolerance)) {
					bottomNote->state = idMusicNote::state_t::MISSED;
				}
			}
		}
	}

	currentLevel.RemoveNotesForTime(timeSinceStepStart - pressLateTolerance);

	// # Score Management
	const std::vector<idMusicNote>& playedNotes = currentLevel.GetPlayedNotes();
	for (int i = 0; i < playedNotes.size(); ++i) {
		const idMusicNote& note = playedNotes[i];
		if (note.state == idMusicNote::state_t::PRESSED) {
			comboCount++;
			int noteValue = std::lround((note.endSeconds - note.startSeconds) * 10);
			score += comboCount * noteValue * 100;
		} else {
			comboCount = 0;
			missedNotes++;
		}
	}
	currentLevel.ClearPlayedNotes();
}

void idGameManager::UpdateGameView() {
	// Draw notes
	view.ClearNotesArea();
	const float& laneLengthSeconds = currentLevel.GetLaneLengthSeconds();
	for (int lane = 0; lane < GAME_LANE_COUNT; ++lane) {
		const std::deque<idMusicNote> &laneNotes = currentLevel.GetReadonlyActiveNotes(lane);
		for (int i = 0; i < laneNotes.size(); ++i) {
			const idMusicNote& note = laneNotes[i];
			view.DrawNote(note, lane, laneLengthSeconds, timeSinceStepStart);
		}
	}
	
	// Draw bottom bar
	bool heldKeys[GAME_LANE_COUNT];
	for (int i = 0; i < GAME_LANE_COUNT; ++i) {
		heldKeys[i] = input.WasKeyHeld(KeyConstants::LANE_KEYS[i]);
	}

	view.DrawBottomBar(heldKeys);
	view.UpdateUI(int(timeSinceStepStart), score, comboCount, missedNotes);

	view.Refresh();
}
