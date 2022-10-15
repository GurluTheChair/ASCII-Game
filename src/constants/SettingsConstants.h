#ifndef __SETTINGS_CONSTANTS__
#define __SETTINGS_CONSTANTS__

namespace GameplaySettingsConstants {
	extern const unsigned int SCORE_MULTIPLIER; // Global score multiplier
	extern const unsigned int BIG_COMBO_LOSS_THRESHOLD; // Minimum combo loss that must be notified to the player
	extern const float NOTE_ERROR_DISPLAY_DURATION; // Duration during which an error on a note is displayed
	extern const float EARLY_PRESS_TOLERANCE_SECONDS; // Maximum valid press time before a note's start
	extern const float LATE_PRESS_TOLERANCE_SECONDS; // Maximum valid press time after a note's start
	extern const float EARLY_RELEASE_TOLERANCE_SECONDS; // Maximum valid release time before a note's end
	extern const float MAX_MISS_TIME_DISTANCE_SECONDS; // Maximum distance at which misses will be counted
}

#endif
