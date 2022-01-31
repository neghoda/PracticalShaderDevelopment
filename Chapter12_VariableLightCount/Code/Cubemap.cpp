#include "Cubemap.h"
#include "stb_image.h"
#include "glad.h"

Cubemap::Cubemap() {
	mWidth = 0;
	mHeight = 0;
	mChannels = 0;
	glGenTextures(1, &mHandle);
}

Cubemap::Cubemap(const char* front,
	const char* back,
	const char* right,
	const char* left,
	const char* top,
	const char* bottom) {
	glGenTextures(1, &mHandle);
	Load(front, back, right, left, top, bottom);
}

Cubemap::~Cubemap() {
	glDeleteTextures(1, &mHandle);
}

void Cubemap::Load(const char* front,
	const char* back,
	const char* right,
	const char* left,
	const char* top,
	const char* bottom) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int width, height, channels;
	unsigned char* data;
	data = stbi_load(right, &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	data = stbi_load(left, &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	data = stbi_load(top, &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	data = stbi_load(bottom, &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	data = stbi_load(front, &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	data = stbi_load(back, &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::Set(unsigned int uniformIndex, unsigned int textureIndex) {
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
	glUniform1i(uniformIndex, textureIndex);
}

void Cubemap::UnSet(unsigned int textureIndex) {
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(GL_TEXTURE0);
}

unsigned int Cubemap::GetHandle() {
	return mHandle;
}