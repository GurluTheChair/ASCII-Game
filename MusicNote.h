#ifndef __MUSIC_NOTE__
#define __MUSIC_NOTE__

class idMusicNote {
	public :
		int column;
		float startSeconds;
		float endSeconds;
		friend std::istream& operator>>(std::istream& is, idMusicNote& note);
};

#endif