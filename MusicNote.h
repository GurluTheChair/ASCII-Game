#ifndef __MUSIC_NOTE__
#define __MUSIC_NOTE__

class idMusicNote {
	public :
		enum state_t {
			ACTIVE,
			PRESSED,
			MISSED,
			INACTIVE
		};
		friend std::istream& operator>>(std::istream& is, idMusicNote& note);

		int column;
		float startSeconds;
		float endSeconds;
		state_t state;

		idMusicNote() = default;
		idMusicNote(int _column, float _startSeconds, float _endSeconds, state_t _state = INACTIVE);
};

#endif