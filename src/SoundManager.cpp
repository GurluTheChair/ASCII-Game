#include <utility>

#include "SoundUtils.h"
#include "SoundManager.h"

idSoundManager::idSoundManager() 
: unplayingSources(INITIAL_SOURCE_COUNT, 0)
, playingSources()
, registeredBuffers() {
	device = alcOpenDevice(NULL); // retrieve default device
	context = alcCreateContext(device, NULL); // create context with no additional attributes
	alcMakeContextCurrent(context);
	
	// Prepare source pools
	alGenSources(INITIAL_SOURCE_COUNT, &unplayingSources[0]);
	for (int i = 0; i < INITIAL_SOURCE_COUNT; i++) {
		InitSource(unplayingSources[i]);
	}
	playingSources.reserve(INITIAL_SOURCE_COUNT);
}

idSoundManager::~idSoundManager() {
	if (unplayingSources.size() > 0) {
		alDeleteSources((ALsizei)unplayingSources.size(), &unplayingSources[0]);
	}

	if (playingSources.size() > 0) {
		alDeleteSources((ALsizei)playingSources.size(), &playingSources[0]);
	}

	for (std::pair<const std::string, ALuint> &p : registeredBuffers) {
		alDeleteBuffers(1, &p.second);
	}

	alcMakeContextCurrent(NULL);

	if (context != NULL) {
		alcDestroyContext(context);
	}

	if (device != NULL) {
		alcCloseDevice(device);
	}
}

bool idSoundManager::LoadWav(const std::string &fileName) {
	// Simply return if file already loaded
	if (registeredBuffers.count(fileName) > 0) {
		return true;
	}

	// Prepare OpenAL buffer for sound data
	ALuint newBuffer;
	alGenBuffers(1, &newBuffer);
	ALenum alError = alGetError();
	if (alError != AL_NO_ERROR) {
		return false;
	}
	
	// Load sound data
	int32_t numChannels, sampleRate, bitsPerSample, dataSize;
	char* soundData = LoadWavFile(fileName, numChannels, sampleRate, bitsPerSample, dataSize);
	if (soundData == nullptr) {
		alDeleteBuffers(1, &newBuffer);
		return false;
	}

	// Compute format
	ALenum format;
	if (numChannels == 1) {
		if (bitsPerSample == 8) {
			format = AL_FORMAT_MONO8;
		} else if (bitsPerSample == 16) {
			format = AL_FORMAT_MONO16;
		} else {
			alDeleteBuffers(1, &newBuffer);
			return false;
		}
	} else if (numChannels == 2) {
		if (bitsPerSample == 8) {
			format = AL_FORMAT_STEREO8;
		}
		else if (bitsPerSample == 16) {
			format = AL_FORMAT_STEREO16;
		}
		else {
			alDeleteBuffers(1, &newBuffer);
			return false;
		}
	} else {
		alDeleteBuffers(1, &newBuffer);
		return false;
	}

	// Fill OpenAL buffer with data
	alBufferData(newBuffer, format, soundData, dataSize, sampleRate);
	delete[] soundData;
	alError = alGetError();
	if (alError != AL_NO_ERROR) {
		alDeleteBuffers(1, &newBuffer);
		return false;
	}

	// Register successfully created buffer
	registeredBuffers[fileName] = newBuffer;

	return true;
}

bool idSoundManager::UnloadFile(const std::string &fileName) {
	if (registeredBuffers.count(fileName) <= 0) {
		return false;
	}

	// Delete OpenAL buffer (fails if buffer is in use)
	ALuint buffer = registeredBuffers.at(fileName);
	alDeleteBuffers(1, &buffer);
	ALenum alError = alGetError();
	if (alError != AL_NO_ERROR) {
		return false;
	}

	registeredBuffers.erase(fileName);
	return true;
}

bool idSoundManager::Play(const std::string &fileName, const bool repeat) {
	if (registeredBuffers.count(fileName) <= 0) {
		return false;
	}

	// Create new source if unplaying source pool is empty
	if (unplayingSources.size() <= 0) {
		ALuint newSource;
		alGenSources(1, &newSource);
		InitSource(newSource);
		unplayingSources.push_back(newSource);
	}

	// Retrieve buffer and source to play sound
	ALuint source = unplayingSources.back();
	ALuint buffer = registeredBuffers.at(fileName);

	// Prepare source
	alSourcei(source, AL_BUFFER, buffer);
	alSourcei(source, AL_LOOPING, repeat? AL_TRUE : AL_FALSE);
	ALenum alError = alGetError();
	if (alError != AL_NO_ERROR) {
		return false;
	}

	// Play sound and transfer source to "playing" container
	alSourcePlay(source);
	unplayingSources.pop_back();
	playingSources.push_back(source);

	return true;
}

void idSoundManager::UpdateSourceStates() {
	size_t playingSize = playingSources.size();
	// If no source is playing, no need to do anything
	if (playingSize <= 0) {
		return;
	}

	// Move stopped sources to unplaying source pool
	ALint sourceState;
	for (int i = int(playingSize - 1); i >= 0; --i) {
		alGetSourcei(playingSources[i], AL_SOURCE_STATE, &sourceState);
		if (sourceState == AL_STOPPED) {
			alSourcei(playingSources[i], AL_BUFFER, NULL);
			unplayingSources.push_back(playingSources[i]);
			playingSources[i] = playingSources.back();
			playingSources.pop_back();
		}
	}
}

void idSoundManager::InitSource(const ALuint &source) {
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1.0f);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
}
