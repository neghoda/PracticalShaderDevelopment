#ifndef _H_SAMPLE_
#define _H_SAMPLE_

#include "vec3.h"
#include "vec2.h"

#include "Application.h"
#include "Shader.h"
#include "Cubemap.h"
#include "Mesh.h"

#define DEG2RAD 0.0174533f

class Sample : public Application {
protected:
	Shader* mShader;
	Mesh* mMesh;
	Cubemap* mCubemap;
	float mRotation;
public:
	void Initialize();
	void Update(float inDeltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};

#endif