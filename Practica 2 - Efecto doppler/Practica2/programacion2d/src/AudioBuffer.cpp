#include <cstdint>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <al.h>
#include <alc.h>
#include <AudioBuffer.h>

AudioBuffer::AudioBuffer(uint32_t alBufferId) {
	mAlBuffer = alBufferId;
};

AudioBuffer::~AudioBuffer() {
	alDeleteBuffers(1, &mAlBuffer);
}

AudioBuffer* AudioBuffer::load(const char* filename) {
	if (filename) {
		std::ifstream i(filename, std::ios::binary);
		if (!i.is_open()) {
			return nullptr;
		}

		i.seekg(0, std::ios::cur);
		std::vector<char>chunkID;
		chunkID.resize(4);
		i.read(chunkID.data(), 4);

		if (chunkID.at(0) == 'R' && chunkID.at(1) == 'I' && chunkID.at(2) == 'F' && chunkID.at(3) == 'F') {
			// Chunk size
			std::vector<char>chunkSize;
			chunkSize.resize(4);
			i.read(chunkSize.data(), 4);
			uint32_t* chunkSizeInt = reinterpret_cast<uint32_t*>(chunkSize.data());

			// Chunk wave
			std::vector<char>chunkWave;
			chunkWave.resize(4);
			i.read(chunkWave.data(), 4);

			// Chunk fmt
			std::vector<char>chunkFmt;
			chunkFmt.resize(4);
			i.read(chunkFmt.data(), 4);

			// Chunk size fmt
			std::vector<char>fmtChunkSize;
			fmtChunkSize.resize(4);
			i.read(fmtChunkSize.data(), 4);
			uint32_t* fmtChunkSizeInt = reinterpret_cast<uint32_t*>(fmtChunkSize.data());

			// Audio format
			std::vector<char>audioFormat;
			audioFormat.resize(4);
			i.read(audioFormat.data(), 2);
			uint16_t* audioFormatInt = reinterpret_cast<uint16_t*>(audioFormat.data());

			// Channels
			std::vector<char>channels;
			channels.resize(4);
			i.read(channels.data(), 2);
			uint16_t* channelsInt = reinterpret_cast<uint16_t*>(channels.data());

			// Sample rate
			std::vector<char>sampleRate;
			sampleRate.resize(4);
			i.read(sampleRate.data(), 4);
			uint32_t* sampleRateInt = reinterpret_cast<uint32_t*>(sampleRate.data());

			// Byte rate
			std::vector<char>byteRate;
			byteRate.resize(4);
			i.read(byteRate.data(), 4);
			uint32_t* byteRateInt = reinterpret_cast<uint32_t*>(byteRate.data());

			// Block align
			std::vector<char>blockAlign;
			blockAlign.resize(4);
			i.read(blockAlign.data(), 2);
			uint16_t* blockAlignInt = reinterpret_cast<uint16_t*>(blockAlign.data());

			// Bits per sample
			std::vector<char>bitsPerSample;
			bitsPerSample.resize(4);
			i.read(bitsPerSample.data(), 2);
			uint16_t* bitsPerSampleInt = reinterpret_cast<uint16_t*>(bitsPerSample.data());

			if (*fmtChunkSizeInt > 16) {
				// Extra params size
				std::vector<char>extraParamsSize;
				extraParamsSize.resize(2);
				i.read(extraParamsSize.data(), 2);
				uint16_t* extraParamsSizeInt = reinterpret_cast<uint16_t*>(extraParamsSize.data());

				std::vector<char>extraParams;
				extraParams.resize(*extraParamsSizeInt);
				i.read(extraParams.data(), *extraParamsSizeInt);
			}

			std::vector<char>dataSize;
			dataSize.resize(4);
			uint32_t* dataSizeInt = nullptr;

			bool dataFinded = false;
			while (!dataFinded) {
				std::vector<char>data;
				data.resize(4);
				i.read(data.data(), 4);
				if (data.at(0) == 'd' && data.at(1) == 'a' && data.at(2) == 't' && data.at(3) == 'a') {
					dataFinded = true;

					i.read(dataSize.data(), 4);
					dataSizeInt = reinterpret_cast<uint32_t*>(dataSize.data());
				}
				else {
					std::vector<char>data2;
					data2.resize(4);
					i.read(data2.data(), 4);
					uint32_t* data2Int = reinterpret_cast<uint32_t*>(data2.data());

					std::vector<char>dataToJump;
					dataToJump.resize(*data2Int);
					i.read(dataToJump.data(), *data2Int);
				}
			}
			
			std::vector<char>wavData;
			
			if (!dataSizeInt) {
				return nullptr;
			}
			
			wavData.resize(*dataSizeInt);
			
			i.read(wavData.data(), *dataSizeInt);

			i.close();
			
			ALuint buffer;
			alGenBuffers(1, &buffer);

			ALenum format;
			if (*channelsInt == 1) {
				if (*bitsPerSampleInt == 8) {
					format = AL_FORMAT_MONO8;
				}
				else {
					format = AL_FORMAT_MONO16;
				}
			}
			else {
				if (*bitsPerSampleInt == 8) {
					format = AL_FORMAT_STEREO8;
				}
				else {
					format = AL_FORMAT_STEREO16;
				}
			}

			alBufferData(buffer, format, wavData.data(), *dataSizeInt, *sampleRateInt);
			
			AudioBuffer *audioBuffer = new AudioBuffer(buffer);
			return audioBuffer;
		}
	}

	return nullptr;
}

uint32_t AudioBuffer::getAlBuffer() const {
	return mAlBuffer;
}