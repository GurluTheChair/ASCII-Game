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
	// TODO : make improvements to make the game more generous and/or precise
	currentLevel.UpdateNotesActiveState(timeSinceStart);
	
	int laneKeys[4] = { 'A', 'Z', 'E', 'R' };
	idMusicNote* bottomNotes[4];
	currentLevel.GetBottomNotes(bottomNotes);

	const float pressEarlyTolerance = 0.1f;
	const float pressLateTolerance = 0.1f;
	const float releaseEarlyTolerance = 0.1f;
	for (int i = 0; i < GAME_LANE_COUNT; ++i)
	{
		if ((bottomNotes[i] != nullptr) && 
			(bottomNotes[i]->state != idMusicNote::state_t::MISSED)) {
			if (input.WasKeyHeld(laneKeys[i])) {
				// TODO: differenciate between press and hold
				if (bottomNotes[i]->state == idMusicNote::state_t::ACTIVE) {
					if ((timeSinceStart >= bottomNotes[i]->startSeconds - pressEarlyTolerance) &&
						(timeSinceStart <= bottomNotes[i]->startSeconds + pressLateTolerance)) {
						bottomNotes[i]->state = idMusicNote::state_t::PRESSED;
					} else {
						bottomNotes[i]->state = idMusicNote::state_t::MISSED;
					}
				}
			} else {
				if ((timeSinceStart > bottomNotes[i]->startSeconds + pressLateTolerance) &&
					(timeSinceStart <= bottomNotes[i]->endSeconds - releaseEarlyTolerance)) {
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
			missedNotes++;
		}
	}
	currentLevel.ClearPlayedNotes();
}

void idGameManager::UpdateGameView() {
	// TODO : replace hard-coded values with variables/constants

	view.ClearGame();

	idViewManager::rectangle_t rectangle;
	WORD noteColor = BACKGROUND_COLOR;
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
					noteColor = NOTE_COLOR;
					break;
				case idMusicNote::state_t::PRESSED:
					noteColor = view.GetPressedColorByLane(lane);
					break;
				case idMusicNote::state_t::MISSED:
					noteColor = MISSED_COLOR;
					break;
				default:
					break;
			}

			view.DrawRectangle(rectangle, BACKGROUND_COLOR, noteColor);
		}
	}
	
	bool heldKeys[GAME_LANE_COUNT];
	int laneKeys[GAME_LANE_COUNT] = { 'A', 'Z', 'E', 'R' };
	for (int i = 0; i < GAME_LANE_COUNT; ++i) {
		heldKeys[i] = input.WasKeyHeld(laneKeys[i]);
	}
	view.DrawBottomBar(heldKeys);
	view.UpdateUI(int(timeSinceStart), score, comboCount, missedNotes, BACKGROUND_COLOR, TEXT_COLOR);
	view.Refresh();
}
