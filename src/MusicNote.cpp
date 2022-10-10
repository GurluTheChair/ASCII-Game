#include "MusicNote.h"

std::istream& operator>>(std::istream &is, idMusicNote &note) {
	is >> note.column;
	is >> note.startSeconds;
	is >> note.endSeconds;
	note.state = idMusicNote::state_t::ACTIVE;
	return is;
}

idMusicNote::idMusicNote(int _column, float _startSeconds, float _endSeconds, state_t _state)
	: column(_column)
	, startSeconds(_startSeconds)
	, endSeconds(_endSeconds)
	, state(_state)
{}
