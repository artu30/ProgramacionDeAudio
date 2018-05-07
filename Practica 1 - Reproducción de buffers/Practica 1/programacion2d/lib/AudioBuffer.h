class AudioBuffer {
public:
	~AudioBuffer();

	static AudioBuffer* load        (const char* filename);
	uint32_t            getAlBuffer ()                      const;
private:
	AudioBuffer(uint32_t alBufferId);
	uint32_t mAlBuffer;
};