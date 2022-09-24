#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <windows.h>

#include "NYTimer.h"
#include "MusicNote.h"
#include "GameLevel.h"
#include "InputManager.h"
#include "ViewManager.h"
#include "GameManager.h"

#include <iostream>

idGameManager::idGameManager(idInputManager& _input, idViewManager& _view, const float _frameRate)
: input(_input)
, view(_view)
, frameRate(_frameRate)
, deltaTime(0.0f)
, timeSinceStart(0.0f)
, comboCount(0)
, score(0)
{ }

bool idGameManager::InitGame(const std::string& levelFilename) {
	bool loadingWorked = currentLevel.LoadFile(levelFilename);

	input.RegisterKey('A');
	input.RegisterKey('Z');
	input.RegisterKey('E');
	input.RegisterKey('R');

	return loadingWorked;
}

void idGameManager::StartGame() {
	NYTimer timer;
	timer.start();
	float startTime = timer.getElapsedSeconds();
	
	const float& songLength = currentLevel.GetLengthSeconds();
	const float delayBetweenFrames = 1.0f / frameRate;

	timeSinceStart = 0.0f;
	comboCount = 0;
	score = 0;

	UpdateGame();
	float previousUpdateTime = startTime;
	float currentLoopTime;

	while (timeSinceStart < songLength) {
		input.UpdateKeyStates();
		currentLoopTime = timer.getElapsedSeconds();

		if (currentLoopTime > (previousUpdateTime + delayBetweenFrames)) {
			timeSinceStart = currentLoopTime - startTime;
			deltaTime = currentLoopTime - previousUpdateTime;

			UpdateGame();
			input.ResetKeyStates();

			previousUpdateTime = currentLoopTime;
		}
		Sleep(1);
	}
}

void idGameManager::UpdateGame() {
	UpdateGameData();
	UpdateGameView();
}

void idGameManager::UpdateGameData() {
	// # Input Management
	// TODO : make improvements to make the game more generous and/or precise
	currentLevel.UpdateNotesActiveState(timeSinceStart);
	
	int laneKeys[4] = { 'A', 'Z', 'E', 'R' };
	idMusicNote* bottomNotes[4];
	currentLevel.GetBottomNotes(bottomNotes);

	const float pressEarlyTolerance = 0.05f;
	const float pressLateTolerance = 0.05f;
	const float releaseEarlyTolerance = 0.05f;
	for (int i = 0; i < GAME_LANE_COUNT; ++i)
	{
		if ((bottomNotes[i] != nullptr) && 
			(bottomNotes[i]->state != idMusicNote::state_t::MISSED)) {
			if (input.WasKeyHeld(laneKeys[i])) {
				if ((timeSinceStart >= bottomNotes[i]->startSeconds - pressEarlyTolerance) &&
					(timeSinceStart <= bottomNotes[i]->startSeconds + pressLateTolerance)) {
					bottomNotes[i]->state = idMusicNote::state_t::PRESSED;
				} else {
					bottomNotes[i]->state = idMusicNote::state_t::MISSED;
				}
			} else {
				if (timeSinceStart > bottomNotes[i]->startSeconds + pressLateTolerance) {
					bottomNotes[i]->state = idMusicNote::state_t::MISSED;
				}
			}
		}
	}

	// # Score Management
	const std::vector<idMusicNote>& playedNotes = currentLevel.GetPlayedNotes();
	for (int i = 0; i < playedNotes.size(); ++i) {
		const idMusicNote& note = playedNotes[i];
		if (note.state == idMusicNote::state_t::ACTIVE) {
			comboCount++;
			score += int(comboCount * (note.endSeconds - note.startSeconds) * 100);
		} else {
			comboCount = 0;
		}
	}
	currentLevel.ClearPlayedNotes();
}

void idGameManager::UpdateGameView() {
	// TODO : replace hard-coded values with variables/constants

	view.Clear();

	idViewManager::rectangle_t rectangle;
	WORD noteColor = 0x0000;
	const float& laneLengthSeconds = currentLevel.GetLaneLengthSeconds();
	for (int lane = 0; lane < GAME_LANE_COUNT; ++lane) {
		const std::deque<idMusicNote> &laneNotes = currentLevel.GetActiveNotes(lane);

		for (int i = 0; i < laneNotes.size(); ++i) {
			const idMusicNote& note = laneNotes[i];

			rectangle.origin_x = lane * LANE_WIDTH;
			rectangle.origin_y = LANE_HEIGHT * (1 + ((timeSinceStart - note.startSeconds) / laneLengthSeconds));
			rectangle.width = LANE_WIDTH;
			rectangle.height = LANE_HEIGHT * ((note.endSeconds - note.startSeconds) / laneLengthSeconds);

			switch (note.state) {
				case idMusicNote::state_t::ACTIVE:
					noteColor = 0x000F;
					break;
				case idMusicNote::state_t::PRESSED:
					noteColor = 0x000E;
					break;
				case idMusicNote::state_t::MISSED:
					noteColor = 0x0008;
					break;
				default:
					break;
			}

			view.DrawRectangle(rectangle, BACKGROUND_COLOR, noteColor);
		}
	}

	view.DrawBoard();
	view.Refresh();
}
