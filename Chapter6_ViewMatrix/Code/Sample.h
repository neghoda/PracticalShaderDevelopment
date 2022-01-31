#ifndef _H_SAMPLE_
#define _H_SAMPLE_

#include "vec3.h"
#include "vec2.h"

#include "Application.h"
#include "Shader.h"
#include "Attribute.h"
#include "IndexBuffer.h"
#include "Texture.h"

#define DEG2RAD 0.0174533f

class Sample : public Application {
protected:
	Shader* mAlienShader;
	Shader* mCloudShader;

	Attribute<vec3>* mVertexPositions;
	Attribute<vec3>* mVertexNormals;
	Attribute<vec2>* mVertexTexCoords;
	IndexBuffer* mIndexBuffer;

	Texture* mAlienTexture;
	Texture* mBackgroundTexture;
	Texture* mCloudTexture;
	Texture* mSunTexture;

	float mRotation;
	float mFrame;
public:
	void Initialize();
	void Update(float inDeltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};

#endif
