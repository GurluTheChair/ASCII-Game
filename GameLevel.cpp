#include <string>
#include <vector>
#include <deque>
#include <istream>
#include <fstream>
#include <algorithm>

#include "MusicNote.h"
#include "GameLevel.h"

#define EXTRACT_LINE_WITH_FAIL_RETURN(istream, string) if (!std::getline(istream, string)) { return false; }
#define EXTRACT_WITH_FAIL_RETURN(istream, variable) if (!(istream >> variable)) { return false; }

bool idGameLevel::LoadFile(const std::string& levelFilename) {
	std::ifstream levelFile(levelFilename);

	// Load main level data
	EXTRACT_LINE_WITH_FAIL_RETURN(levelFile, songFilename)
	EXTRACT_LINE_WITH_FAIL_RETURN(levelFile, songName)
	EXTRACT_WITH_FAIL_RETURN(levelFile, lengthSeconds)
	EXTRACT_WITH_FAIL_RETURN(levelFile, laneLengthSeconds)
	size_t notesCount;
	EXTRACT_WITH_FAIL_RETURN(levelFile, notesCount)

	// Clear previously loaded notes (if any)
	unplayedNotes.clear();
	for (int i = 0; i < GAME_LANE_COUNT; ++i)
		activeNotes[i].clear();

	// Load notes data
	unplayedNotes.reserve(notesCount);
	idMusicNote note;
	while (!levelFile.eof()) {
		EXTRACT_WITH_FAIL_RETURN(levelFile, note)
		unplayedNotes.push_back(note);
	}
	// Sort notes in descending order
	std::sort(unplayedNotes.begin(), unplayedNotes.end(), std::greater<idMusicNote>());

	return levelFile.fail();
}

const std::deque<idMusicNote>& idGameLevel::GetActiveNotes(const unsigned int lane) const {
	return activeNotes[lane];
}

void idGameLevel::GetBottomNotes(idMusicNote** output) {
	for (int i = 0; i < GAME_LANE_COUNT; i++) {
		if (activeNotes[i].size() > 0)
			output[i] = &activeNotes[i].front();
		else
			output[i] = nullptr;
	}
}

void idGameLevel::UpdateNotesActiveState(const float time) {
	for (int i = 0; i < GAME_LANE_COUNT; i++) {
		while (time > activeNotes[i].front().endSeconds) {
			activeNotes[i].pop_front();
		}
	}

	idMusicNote nextNote = unplayedNotes.back();
	while (nextNote.startSeconds <= time) {
		activeNotes[nextNote.column].push_back(nextNote);
		unplayedNotes.pop_back();
		nextNote = unplayedNotes.back();
	};
}
