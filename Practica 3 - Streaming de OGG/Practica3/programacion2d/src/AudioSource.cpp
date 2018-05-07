#include <cstdint>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <al.h>
#include <alc.h>
#include <AudioBuffer.h>
#include <AudioSource.h>
#include <AudioStream.h>

AudioSource::AudioSource(AudioBuffer* buffer) {
	mBuffer = buffer;

	mLoop  = false;
	mPitch = 1.0f;
	mGain  = 1.0f;

	mPosition.mX = 0.0f;
	mPosition.mY = 0.0f;
	mPosition.mZ = 0.0f;

	mVelocity.mX = 0.0f;
	mVelocity.mY = 0.0f;
	mVelocity.mZ = 0.0f;

	alGenSources(1, &mSource);

	alSourcef  (mSource, AL_PITCH,    mPitch);
	alSourcef  (mSource, AL_GAIN,     mGain);
	alSourcef  (mSource, AL_MIN_GAIN, 0.0f);
	alSourcef  (mSource, AL_MAX_GAIN, 1.0f);
	alSource3f (mSource, AL_POSITION, mPosition.mX, mPosition.mY, mPosition.mZ);
	alSource3f (mSource, AL_VELOCITY, mVelocity.mX, mVelocity.mY, mVelocity.mZ);
	alSourcei  (mSource, AL_LOOPING,  mLoop);
	alSourcei  (mSource, AL_BUFFER,   mBuffer->getAlBuffer());
}

void AudioSource::setPitch(float pitch) {
	mPitch = pitch;
	alSourcef(mSource, AL_PITCH, mPitch);
}

void AudioSource::setGain(float gain) {
	mGain = gain;
	alSourcef(mSource, AL_GAIN, mGain);
}

void AudioSource::setLooping(bool loop) {
	mLoop = loop;
	alSourcei(mSource, AL_LOOPING, mLoop);
}

void AudioSource::setPosition(float x, float y, float z) {
	mPosition.mX = x;
	mPosition.mY = y;
	mPosition.mZ = z;
	alSource3f(mSource, AL_POSITION, mPosition.mX, mPosition.mY, mPosition.mZ);
}

void AudioSource::setVelocity(float x, float y, float z) {
	mVelocity.mX = x;
	mVelocity.mY = y;
	mVelocity.mZ = z;
	alSource3f(mSource, AL_VELOCITY, mVelocity.mX, mVelocity.mY, mVelocity.mZ);
}

void AudioSource::play() {
	alSourcePlay(mSource);
}

void AudioSource::stop() {
	alSourceStop(mSource);
}

void AudioSource::pause() {
	alSourcePause(mSource);
}

bool AudioSource::isPlaying() const {
	ALint status;
	alGetSourcei(mSource, AL_SOURCE_STATE, &status);

	if (status == AL_PLAYING) {
		return true;
	}

	return false;
}

Vec3D AudioSource::getPosition() {
	return mPosition;
}

Vec3D AudioSource::getVelocity() {
	return mVelocity;
}

static AudioSource* load(const char* filename) {


	return nullptr;
}