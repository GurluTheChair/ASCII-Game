#include <istream>

#include "MusicNote.h"

idMusicNote::idMusicNote(int _column, float _startSeconds, float _endSeconds, state_t _state)
	: column(_column)
	, startSeconds(_startSeconds)
	, endSeconds(_endSeconds)
	, state(_state)
{}

std::istream& operator>>(std::istream& is, idMusicNote& note) {
	// TODO : write the code
	return is;
}

bool idMusicNote::operator<(const idMusicNote& other) {
	return startSeconds < other.startSeconds;
}

bool idMusicNote::operator>(const idMusicNote& other) {
	return startSeconds > other.startSeconds;
}