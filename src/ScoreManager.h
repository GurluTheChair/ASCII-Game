#ifndef __SCORE_MANAGER__
#define __SCORE_MANAGER__

#include <string>
#include <unordered_map>

#include "MusicNote.h"

class idScoreManager {
	public:
		idScoreManager();

		bool LoadHighScores(const std::string &fileName);
		bool SaveHighScores(const std::string &fileName) const;
		void Reset();
		void RegisterHit(const float hitMultiplier);
		void RegisterMiss();
		const bool IsHighScore(const std::string &levelFileName) const;
		const void UpdateHighScore(const std::string &levelFileName);
		
		const unsigned int GetHighScore(const std::string &levelFileName) const;
		const unsigned int GetComboCount() const;
		const unsigned int GetMaxComboCount() const;
		const unsigned int GetMissedNotesCount() const;
		const unsigned int GetPlayedNotesCount() const;
		const unsigned int GetScore() const;
		const float GetAccuracy() const;
		const bool IsFullCombo() const;
	private:
		unsigned int comboCount;
		unsigned int maxComboCount;
		unsigned int missedNotesCount;
		unsigned int playedNotesCount;
		unsigned int score;
		std::unordered_map<std::string, unsigned int> levelHighScores;
};

#endif
