#ifndef _H_SAMPLE_
#define _H_SAMPLE_

#include "vec3.h"

#include "Application.h"
#include "Shader.h"
#include "Attribute.h"

class Sample : public Application {
protected:
	Shader* mShader;
	Attribute<vec3>* mVertexPositions;
public:
	void Initialize();
	void Update(float inDeltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};

#endif
