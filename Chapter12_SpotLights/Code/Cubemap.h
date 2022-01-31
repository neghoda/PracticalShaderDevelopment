#pragma once

class Cubemap {
protected:
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mChannels;
	unsigned int mHandle;
private:
	Cubemap(const Cubemap& other);
	Cubemap& operator=(const Cubemap& other);
public:
	Cubemap();
	Cubemap(const char* front,
		const char* back,
		const char* right,
		const char* left,
		const char* top,
		const char* bottom);
	~Cubemap();

	void Load(const char* front,
		const char* back,
		const char* right,
		const char* left,
		const char* top,
		const char* bottom);

	void Set(unsigned int uniformIndex, unsigned int textureIndex);
	void UnSet(unsigned int textureIndex);
	unsigned int GetHandle();
};
