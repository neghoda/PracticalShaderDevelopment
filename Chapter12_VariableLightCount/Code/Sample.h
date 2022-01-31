#ifndef _H_SAMPLE_
#define _H_SAMPLE_

#include "vec3.h"
#include "vec2.h"

#include "Application.h"
#include "Shader.h"
#include "Attribute.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Mesh.h"
#include "Cubemap.h"

#define DEG2RAD 0.0174533f

class Sample : public Application {
protected:
	Shader* mShaderDirectLight;
	Shader* mShaderPointLight;
	Attribute<vec3>* mVertexPositions;
	Attribute<vec3>* mVertexNormals;
	Attribute<vec2>* mVertexTexCoords;
	IndexBuffer* mIndexBuffer;
	Texture* mDisplayTexture;
	Texture* mSpecularTexture;
	Texture* mNormalTexture;
	Mesh* mMesh;
	
	Shader* mWaterShaderDirectLight;
	Shader* mWaterShaderPointLight;
	Mesh* mWaterMesh;
	Texture* mWaterNormalTexture;

	Shader* mSkyboxShader;
	Mesh* mSkyboxMesh;
	Cubemap* mSkyboxCubemap;

	float mRotation;
	float mTime;
public:
	void Initialize();
	void Update(float inDeltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};

#endif