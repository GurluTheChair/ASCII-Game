#include "NYTimer.h"
#include "MusicNote.h"

#include "GameManager.h"

idGameManager::idGameManager(idInputManager& _input, idViewManager& _view, idSoundManager& _sound, const float _frameRate)
: input(_input)
, view(_view)
, sound(_sound)
, frameRate(_frameRate)
, deltaTime(0.0f)
, timeSinceStart(0.0f)
, comboCount(0)
, score(0)
, missedNotes(0)
{ }

bool idGameManager::InitGame(const std::string& levelFilename) {
	bool loadingWorked = currentLevel.LoadFile(levelFilename);

	input.RegisterKey('A');
	input.RegisterKey('Z');
	input.RegisterKey('E');
	input.RegisterKey('R');

	view.DrawUIBorder(BACKGROUND_COLOR, TEXT_COLOR);

	return loadingWorked;
}

void idGameManager::StartGame() {
	// TODO : replace hard-coded value with constant
	std::string songFilePath(".\\songs\\");
	songFilePath.append(currentLevel.GetSongFilename());

	if (!sound.LoadWav(songFilePath))
		return;

	const float& songLength = currentLevel.GetLengthSeconds();
	const float delayBetweenFrames = 1.0f / frameRate;

	timeSinceStart = 0.0f;
	comboCount = 0;
	missedNotes = 0;
	score = 0;

	view.DrawUI(currentLevel.GetSongName(), int(currentLevel.GetLengthSeconds()), BACKGROUND_COLOR, TEXT_COLOR);
	UpdateGame();

	NYTimer timer;
	timer.start();
	float startTime = timer.getElapsedSeconds();

	float previousUpdateTime = startTime;
	float currentLoopTime;

	if (!sound.Play(songFilePath))
		return;

	while (timeSinceStart < songLength) {
		input.UpdateKeyStates();
		currentLoopTime = timer.getElapsedSeconds();

		if (currentLoopTime > (previousUpdateTime + delayBetweenFrames)) {
			timeSinceStart = currentLoopTime - startTime;
			deltaTime = currentLoopTime - previousUpdateTime;

			UpdateGame();
			sound.UpdateSourceStates();
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
	currentLevel.ActivateNotesForTime(timeSinceStart);
	
	int laneKeys[4] = { 'A', 'Z', 'E', 'R' };

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
			(timeSinceStart > bottomNote->endSeconds - releaseEarlyTolerance));

		// Update note state 
		if (bottomNote->state != idMusicNote::state_t::MISSED) {
			if (bottomNote->state == idMusicNote::state_t::ACTIVE) {
				if (timeSinceStart > bottomNote->startSeconds + pressLateTolerance) {
					bottomNote->state = idMusicNote::state_t::MISSED;
				} else if (input.WasKeyPressed(laneKeys[i])) {
					if (timeSinceStart >= bottomNote->startSeconds - pressEarlyTolerance) {
						bottomNote->state = idMusicNote::state_t::PRESSED;
					}
					else if (timeSinceStart + maxMissTimeDistance >= bottomNote->startSeconds - pressEarlyTolerance) {
						bottomNote->state = idMusicNote::state_t::MISSED;
					}
				}
			} else if (bottomNote->state == idMusicNote::state_t::PRESSED) {
				if (input.WasKeyReleased(laneKeys[i]) && 
					(timeSinceStart <= bottomNote->endSeconds - releaseEarlyTolerance)) {
					bottomNote->state = idMusicNote::state_t::MISSED;
				}
			}
		}
	}

	currentLevel.RemoveNotesForTime(timeSinceStart - pressLateTolerance);

	// # Score Management
	const std::vector<idMusicNote>& playedNotes = currentLevel.GetPlayedNotes();
	for (int i = 0; i < playedNotes.size(); ++i) {
		const idMusicNote& note = playedNotes[i];
		if (note.state == idMusicNote::state_t::PRESSED) {
			comboCount++;
			score += int(comboCount * (note.endSeconds - note.startSeconds) * 1000);
		} else {
			comboCount = 0;
			missedNotes++;
		}
	}
	currentLevel.ClearPlayedNotes();
}

void idGameManager::UpdateGameView() {
	// TODO : replace hard-coded values with variables/constants

	// Draw notes
	view.ClearNotesArea();
	const float& laneLengthSeconds = currentLevel.GetLaneLengthSeconds();
	for (int lane = 0; lane < GAME_LANE_COUNT; ++lane) {
		const std::deque<idMusicNote> &laneNotes = currentLevel.GetReadonlyActiveNotes(lane);
		for (int i = 0; i < laneNotes.size(); ++i) {
			const idMusicNote& note = laneNotes[i];
			view.DrawNote(note, lane, laneLengthSeconds, timeSinceStart);
		}
	}
	
	// Draw bottom bar
	bool heldKeys[GAME_LANE_COUNT];
	int laneKeys[GAME_LANE_COUNT] = { 'A', 'Z', 'E', 'R' };
	for (int i = 0; i < GAME_LANE_COUNT; ++i) {
		heldKeys[i] = input.WasKeyHeld(laneKeys[i]);
	}

	view.DrawBottomBar(heldKeys);
	view.UpdateUI(int(timeSinceStart), score, comboCount, missedNotes, BACKGROUND_COLOR, TEXT_COLOR);

	view.Refresh();
}
