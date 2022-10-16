#include <fstream>
#include <algorithm>

#include "constants/GameConstants.h"
#include "GameLevel.h"

#define EXTRACT_LINE_WITH_FAIL_RETURN(istream, string) if (!std::getline(istream, string)) { return false; }
#define EXTRACT_WITH_FAIL_RETURN(istream, variable) if (!(istream >> variable)) { return false; }

static bool HighestStartSeconds(const idMusicNote &left, const idMusicNote &right) {
	return left.startSeconds > right.startSeconds;
}

static bool LowestEndSeconds(const idMusicNote &left, const idMusicNote &right) {
	return left.endSeconds < right.endSeconds;
}

idGameLevel::idGameLevel()
: songName("")
, audioFileName("")
, lengthSeconds(0)
, laneLengthSeconds(0) {}

bool idGameLevel::LoadFile(const std::string &levelFileName) {
	std::ifstream levelFile(levelFileName);

	// Load main level data
	EXTRACT_LINE_WITH_FAIL_RETURN(levelFile, songName)
	EXTRACT_LINE_WITH_FAIL_RETURN(levelFile, audioFileName)
	EXTRACT_WITH_FAIL_RETURN(levelFile, lengthSeconds)
	EXTRACT_WITH_FAIL_RETURN(levelFile, laneLengthSeconds)
	size_t notesCount;
	EXTRACT_WITH_FAIL_RETURN(levelFile, notesCount)

	// Clear previously loaded notes (if any)
	unplayedNotes.clear();
	playedNotes.clear();
	for (int i = 0; i < GAME_LANE_COUNT; ++i) {
		activeNotes[i].clear();
	}

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

void idGameLevel::ActivateNotesForTime(const float time) {
	if (unplayedNotes.size() > 0) {
		idMusicNote nextNote = unplayedNotes.back();
		while (nextNote.startSeconds < (time + laneLengthSeconds)) {
			activeNotes[nextNote.column].push_back(nextNote);
			unplayedNotes.pop_back();

			if (unplayedNotes.size() > 0)
				nextNote = unplayedNotes.back();
			else
				break;
		};
	}
}

void idGameLevel::RemoveNotesForTime(const float time, const float tolerance) {
	// Remove notes that can't be played anymore
	for (int lane = 0; lane < GAME_LANE_COUNT; ++lane) {
		std::deque<idMusicNote> &laneActiveNotes = activeNotes[lane];
		std::deque<idMusicNote>::iterator i = laneActiveNotes.begin();

		while (i != laneActiveNotes.end()) {
			if ((time - tolerance > i->endSeconds) || ((i->state == idMusicNote::state_t::PRESSED) && (time > i->endSeconds))) {
				playedNotes.push_back(*i);
				i = laneActiveNotes.erase(i);
			} else {
				++i;
			}
		}
	}
	// Sort the notes that have been played (= been removed)
	if (playedNotes.size() > 1) {
		std::sort(playedNotes.begin(), playedNotes.end(), LowestEndSeconds);
	}
}

const std::deque<idMusicNote>& idGameLevel::GetReadonlyActiveNotes(const unsigned int lane) const {
	return activeNotes[lane];
}

std::deque<idMusicNote>& idGameLevel::GetEditableActiveNotes(const unsigned int lane) {
	return activeNotes[lane];
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

const std::string& idGameLevel::GetAudioFileName() const {
	return audioFileName;
}

const float& idGameLevel::GetLengthSeconds() const {
	return lengthSeconds;
}

const float& idGameLevel::GetLaneLengthSeconds() const {
	return laneLengthSeconds;
}
