class AudioStream {
public: 
	static AudioStream* load(const char* filename, AudioSource* source); 
	
	~AudioStream();

	void setLooping(bool looping) { mShouldLoop = looping; }

	static void updateAll(); 

protected:
	AudioStream(); 
	void update(); 
	bool stream(uint32_t buffer); 

private: 
	static std::vector<AudioStream*> mStreams;
	AudioSource* mSource; 
	stb_vorbis* mStream; 
	stb_vorbis_info mInfo; 
	std::array<uint32_t, 2> mBuffers;
	size_t mSamplesLeft; 
	bool mShouldLoop;
};