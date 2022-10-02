#ifndef __GAME_LEVEL__
#define __GAME_LEVEL__

#include <string>
#include <vector>
#include <deque>

#include "MusicNote.h"

#define GAME_LANE_COUNT 4

class idGameLevel {
	public:
		idGameLevel() = default;
		
		bool LoadFile(const std::string& levelFilename);
		void UpdateNotesActiveState(const float time);

		const std::deque<idMusicNote>& GetActiveNotes(const unsigned int lane) const;
		void GetBottomNotes(idMusicNote **output);
		const std::vector<idMusicNote>& GetPlayedNotes() const;
		void ClearPlayedNotes();

		const std::string& GetSongName() const;
		const std::string& GetSongFilename() const;
		const float& GetLengthSeconds() const;
		const float& GetLaneLengthSeconds() const;
	private:
		std::string songName;
		std::string songFilename;
		float lengthSeconds;
		float laneLengthSeconds;
		std::vector<idMusicNote> unplayedNotes;
		std::deque<idMusicNote> activeNotes[GAME_LANE_COUNT];
		std::vector<idMusicNote> playedNotes;
};

#endif
