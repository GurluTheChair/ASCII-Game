#ifndef __MUSIC_NOTE__
#define __MUSIC_NOTE__

#include <istream>

class idMusicNote {
	public :
		enum class state_t {
			ACTIVE,
			PRESSED,
			MISSED
		};
		friend std::istream& operator>>(std::istream& is, idMusicNote& note);

		int column;
		float startSeconds;
		float endSeconds;
		state_t state;

		idMusicNote() = default;
		idMusicNote(int _column, float _startSeconds, float _endSeconds, state_t _state = state_t::ACTIVE);
};

#endif