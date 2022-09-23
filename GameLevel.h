#ifndef __GAME_LEVEL__
#define __GAME_LEVEL__

#define GAME_LANE_COUNT 4

class idGameLevel {
	public:
		idGameLevel() = default;
		bool LoadFile(const std::string& levelFilename);
		const std::deque<idMusicNote>& GetActiveNotes(const unsigned int lane) const;
		void GetBottomNotes(idMusicNote **output);

	private:
		std::string songName;
		std::string songFilename;
		float lengthSeconds;
		float laneLengthSeconds;
		std::vector<idMusicNote> unplayedNotes;
		std::deque<idMusicNote> activeNotes[GAME_LANE_COUNT];

		void UpdateNotesActiveState(const float time);
};

#endif
