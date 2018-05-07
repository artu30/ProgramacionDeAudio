struct Vec3D {
	float mX;
	float mY;
	float mZ;

	Vec3D ()                          { mX = 0.0f; mY = 0.0f; mZ = 0.0f; }
	Vec3D (float x, float y, float z) { mX = x; mY = y; mZ = z;          }

	void setX(float x) {
		mX = x;
	}
	void setY(float y) {
		mY = y;
	}
	void setZ(float z) {
		mZ = z;
	}
};

class AudioSource {
public:
	AudioSource(AudioBuffer* buffer);

	void setPitch    (float pitch); 
	void setGain     (float gain); 
	void setLooping  (bool loop); 
	void setPosition (float x, float y, float z); 
	void setVelocity (float x, float y, float z);

	Vec3D getPosition();

	void play      (); 
	void stop      (); 
	void pause     (); 
	bool isPlaying () const;
private:
	AudioBuffer*  mBuffer;
	uint32_t      mSource;
	float         mPitch;
	float         mGain;
	bool          mLoop;
	Vec3D         mPosition;
	Vec3D         mVelocity;
};