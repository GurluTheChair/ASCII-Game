#ifndef __SOUND_MANAGER__
#define __SOUND_MANAGER__

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <string>
#include <vector>
#include <unordered_map>

class idSoundManager {
	public:
		idSoundManager();
		~idSoundManager();

		bool LoadWav(const std::string &fileName);
		bool UnloadFile(const std::string &fileName);
		bool Play(const std::string &fileName, const bool repeat=false);
		void UpdateSourceStates();
	private:
		static const uint32_t INITIAL_SOURCE_COUNT = 16;

		ALCdevice* device;
		ALCcontext* context;
		std::vector<ALuint> unplayingSources;
		std::vector<ALuint> playingSources;
		std::unordered_map<std::string, ALuint> registeredBuffers;
		
		void InitSource(const ALuint &source);
};

#endif