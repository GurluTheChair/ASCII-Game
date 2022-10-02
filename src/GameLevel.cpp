#include <fstream>
#include <algorithm>

#include "GameLevel.h"

#define EXTRACT_LINE_WITH_FAIL_RETURN(istream, string) if (!std::getline(istream, string)) { return false; }
#define EXTRACT_WITH_FAIL_RETURN(istream, variable) if (!(istream >> variable)) { return false; }

static bool HighestStartSeconds(const idMusicNote &left, const idMusicNote &right) {
	return left.startSeconds > right.startSeconds;
}

static bool LowestEndSeconds(const idMusicNote &left, const idMusicNote &right) {
	return left.endSeconds < right.endSeconds;
}

bool idGameLevel::LoadFile(const std::string &levelFilename) {
	std::ifstream levelFile(levelFilename);

	// Load main level data
	EXTRACT_LINE_WITH_FAIL_RETURN(levelFile, songName)
	EXTRACT_LINE_WITH_FAIL_RETURN(levelFile, songFilename)
	EXTRACT_WITH_FAIL_RETURN(levelFile, lengthSeconds)
	EXTRACT_WITH_FAIL_RETURN(levelFile, laneLengthSeconds)
	size_t notesCount;
	EXTRACT_WITH_FAIL_RETURN(levelFile, notesCount)

	// Clear previously loaded notes (if any)
	unplayedNotes.clear();
	playedNotes.clear();
	for (int i = 0; i < GAME_LANE_COUNT; ++i)
		activeNotes[i].clear();

	// Load notes data
	unplayedNotes.reserve(notesCount);
	idMusicNote note;
	while (!levelFile.eof()) {
		EXTRACT_WITH_FAIL_RETURN(levelFile, note)
		unplayedNotes.push_back(note);
		levelFile >> std::ws;
	}
	// Sort notes in descending order
	std::sort(unplayedNotes.begin(), unplayedNotes.end(), HighestStartSeconds);

	return !levelFile.fail();
}

void idGameLevel::UpdateNotesActiveState(const float time) {
	for (int i = 0; i < GAME_LANE_COUNT; i++) {
		while ((activeNotes[i].size() > 0) && (time > activeNotes[i].front().endSeconds)) {
			playedNotes.push_back(activeNotes[i].front());
			activeNotes[i].pop_front();
		}
	}

	if (unplayedNotes.size() > 0) {
		idMusicNote nextNote = unplayedNotes.back();
		while (nextNote.startSeconds >= (time - laneLengthSeconds)) {
			activeNotes[nextNote.column].push_back(nextNote);
			unplayedNotes.pop_back();

			if (unplayedNotes.size() > 0)
				nextNote = unplayedNotes.back();
			else
				break;
		};
	}

	if (playedNotes.size() > 1) {
		std::sort(playedNotes.begin(), playedNotes.end(), LowestEndSeconds);
	}
}

const std::deque<idMusicNote>& idGameLevel::GetActiveNotes(const unsigned int lane) const {
	return activeNotes[lane];
}

void idGameLevel::GetBottomNotes(idMusicNote **output) {
	for (int i = 0; i < GAME_LANE_COUNT; i++) {
		if (activeNotes[i].size() > 0)
			output[i] = &activeNotes[i].front();
		else
			output[i] = nullptr;
	}
}

const std::vector<idMusicNote>& idGameLevel::GetPlayedNotes() const {
	return playedNotes;
}

void idGameLevel::ClearPlayedNotes() {
	playedNotes.clear();
}

const std::string& idGameLevel::GetSongName() const {
	return songName;
}

const std::string& idGameLevel::GetSongFilename() const {
	return songFilename;
}

const float& idGameLevel::GetLengthSeconds() const {
	return lengthSeconds;
}

const float& idGameLevel::GetLaneLengthSeconds() const {
	return laneLengthSeconds;
}
