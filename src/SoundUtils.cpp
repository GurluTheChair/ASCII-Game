#include <fstream>

#include "SoundUtils.h"

static int32_t CharArrayToInt(const char* const array, const size_t size) {
	int32_t res = 0;
	// Assumes we're on a little-endian machine (such as on Windows)
	std::memcpy(&res, array, size);
	return res;
}

static bool LoadWavFileHeader(
	std::ifstream &file, 
	int32_t &numChannels,
	int32_t &sampleRate,
	int32_t &bitsPerSample,
	int32_t &dataSize) {
	char buffer[4];

	// Read and check ChunkID
	if (file.read(buffer, 4).fail() || std::strncmp(buffer, "RIFF", 4)) {
		return false;
	}

	// Read ChunkSize
	if (file.read(buffer, 4).fail()) {
		return false;
	}

	// Read and check Format
	if (file.read(buffer, 4).fail() || std::strncmp(buffer, "WAVE", 4)) {
		return false;
	}

	// Read and check Subchunk1ID
	if (file.read(buffer, 4).fail() || std::strncmp(buffer, "fmt ", 4)) {
		return false;
	}

	// Read and check Subchunk1Size (20 in oct is 16 in dec)
	if (file.read(buffer, 4).fail() || std::strncmp(buffer, "\20\0\0\0", 4)) {
		return false;
	}

	// Read AudioFormat
	if (file.read(buffer, 2).fail()) {
		return false;
	}

	// Read and extract NumChannels
	if (file.read(buffer, 2).fail()) {
		return false;
	}
	numChannels = CharArrayToInt(buffer, 2);

	// Read and extract SampleRate
	if (file.read(buffer, 4).fail()) {
		return false;
	}
	sampleRate = CharArrayToInt(buffer, 4);

	// Read and extract ByteRate
	if (file.read(buffer, 4).fail()) {
		return false;
	}
	int32_t byteRate = CharArrayToInt(buffer, 4);

	// Read and extract BlockAlign
	if (file.read(buffer, 2).fail()) {
		return false;
	}
	int32_t blockAlign = CharArrayToInt(buffer, 2);

	// Read and extract BitsPerSample
	if (file.read(buffer, 2).fail()) {
		return false;
	}
	bitsPerSample = CharArrayToInt(buffer, 2);

	// Check values of ByteRate / BlockAlign / BitsPerSample
	if ((byteRate != sampleRate * numChannels * bitsPerSample / 8) ||
		(blockAlign != numChannels * bitsPerSample / 8)) {
		return false;
	}

	// Read and check Subchunk2ID
	if (file.read(buffer, 4).fail() || std::strncmp(buffer, "data", 4)) {
		return false;
	}

	// Read and extract Subchunk2Size (number of bytes in data)
	if (file.read(buffer, 4).fail()) {
		return false;
	}
	dataSize = CharArrayToInt(buffer, 4);

	return true;
}

// Load WAVE file following the specification : http://soundfile.sapp.org/doc/WaveFormat/
// (only works for files following the canonical WAVE format with 2 subchunks)
char* LoadWavFile(
	const std::string &fileName, 
	int32_t &numChannels,
	int32_t &sampleRate,
	int32_t &bitsPerSample,
	int32_t &dataSize) {
	std::ifstream file(fileName, std::ios_base::binary);
	if (file.fail() || !file.is_open())
		return nullptr;

	if (!LoadWavFileHeader(file, numChannels, sampleRate, bitsPerSample, dataSize)) {
		return nullptr;
	}

	char* data = new char[dataSize];
	if (file.read(data, dataSize).fail()) {
		delete[] data;
		return nullptr;
	}

	return data;
}
