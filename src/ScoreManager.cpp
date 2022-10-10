#include <fstream>
#include <utility>

#include "constants/SettingsConstants.h"
#include "ScoreManager.h"

idScoreManager::idScoreManager()
: comboCount(0)
, missedNotesCount(0)
, playedNotesCount(0)
, score(0)
, levelHighScores() {}

bool idScoreManager::LoadHighScores(const std::string &fileName) {
	std::ifstream file(fileName);
	if (!file.good() || !file.is_open()) {
		return true; // File does not exist, do nothing
	}
	if (file.peek() == std::ios::traits_type::eof()) {
		return true; // File is empty, do nothing
	}

	std::string levelFileName;
	unsigned int levelHighScore = 0;
	levelHighScores.clear();
	while (!file.eof()) {
		file >> levelFileName;
		if (file.fail()) {
			return true; // Fail at file name retrieval, we assume it's the end of file
		}
		file >> levelHighScore;
		if (file.fail()) {
			return false; // Fail at high score retrieval, the file is invalid
		}
		levelHighScores[levelFileName] = levelHighScore;
	}

	return true;
}

bool idScoreManager::SaveHighScores(const std::string &fileName) const {
	std::ofstream file(fileName);
	if (!file.good() || !file.is_open()) {
		return false; // File could not be opened
	}

	// Write registered high scores into file
	for (std::pair<const std::string, unsigned int> elem : levelHighScores) {
		file << elem.first << " " << elem.second << "\n";
	}
	file.close(); // Close and flush the file to be able to check for errors

	return !file.fail();
}


void idScoreManager::Reset() {
	comboCount = 0;
	missedNotesCount = 0;
	playedNotesCount = 0;
	score = 0;
}

void idScoreManager::RegisterPlayedNote(const idMusicNote &note) {
	// TODO: replace 100 with game constant
	if (note.state == idMusicNote::state_t::PRESSED) {
		comboCount++;
		int noteValue = std::lround((note.endSeconds - note.startSeconds) * 10);
		score += comboCount * noteValue * GameplaySettingsConstants::SCORE_MULTIPLIER;
	}
	else {
		comboCount = 0;
		missedNotesCount++;
	}
	playedNotesCount++;
}

const bool idScoreManager::CheckForHighScore(const std::string &levelFileName) {
	bool isNewHighScore = (levelHighScores.count(levelFileName) <= 0) || (score > levelHighScores.at(levelFileName));

	if (isNewHighScore) {
		levelHighScores[levelFileName] = score;
	}

	return isNewHighScore;
}

const unsigned int idScoreManager::GetHighScore(const std::string &levelFileName) const {
	if (levelHighScores.count(levelFileName) <= 0) {
		return 0;
	} else {
		return levelHighScores.at(levelFileName);
	}
}

const unsigned int idScoreManager::GetComboCount() const {
	return comboCount;
}

const unsigned int idScoreManager::GetMissedNotesCount() const {
	return missedNotesCount;
}

const unsigned int idScoreManager::GetScore() const {
	return score;
}

const unsigned int idScoreManager::GetPlayedNotesCount() const {
	return playedNotesCount;
}

const float idScoreManager::GetAccuracy() const {
	if (playedNotesCount == 0) {
		return 0.0f;
	} else {
		return float(playedNotesCount - missedNotesCount) / float(playedNotesCount);
	}
}