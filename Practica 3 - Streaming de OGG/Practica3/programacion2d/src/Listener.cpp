#include <al.h>
#include <alc.h>
#include <Listener.h>

void setListenerPosition(float x, float y, float z) {
	alListener3f(AL_POSITION, x, y, z);
}
void setListenerVelocity(float x, float y, float z) {
	alListener3f(AL_VELOCITY, x, y, z);
}
void setListenerOrientation(float x, float y, float z) {
	alListener3f(AL_ORIENTATION, x, y, z);
}