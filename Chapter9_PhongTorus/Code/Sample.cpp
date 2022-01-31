#include "Sample.h"
#include "mat4.h"
#include "quat.h"
#include "Uniform.h"
#include "Draw.h"
#include "PLYHelpers.h"

void Sample::Initialize() {
	mMesh = PLYHelpers::LoadPLYFile("Assets/torus.ply");
	mMesh->UpdateOpenGLBuffers();

	mRotation = 0.0f;
	mShader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");
	mDisplayTexture = new Texture("Assets/uv.png");
}

void Sample::Update(float inDeltaTime) {
	mRotation += inDeltaTime * 45.0f;
	while (mRotation > 360.0f) {
		mRotation -= 360.0f;
	}
}

void Sample::Render(float inAspectRatio) {
	vec3 cameraPosition = vec3(0, 0, -5);
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(cameraPosition, vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 model = quatToMat4(angleAxis(mRotation * DEG2RAD, vec3(1, 1, 1)));

	mShader->Bind();

	Uniform<mat4>::Set(mShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mShader->GetUniform("projection"), projection);
	Uniform<vec3>::Set(mShader->GetUniform("light"), vec3(0, 0, -1));
	Uniform<vec4>::Set(mShader->GetUniform("lightColor"), vec4(1, 0, 0, 1));
	Uniform<vec4>::Set(mShader->GetUniform("ambientColor"), vec4(0, 0, 1, 1));
	Uniform<vec3>::Set(mShader->GetUniform("cameraPosition"), cameraPosition);

	mDisplayTexture->Set(mShader->GetUniform("tex0"), 0);

	mMesh->Bind(mShader->GetAttribute("position"), mShader->GetAttribute("normal"), mShader->GetAttribute("texCoord"));
	mMesh->Draw();
	mMesh->UnBind(mShader->GetAttribute("position"), mShader->GetAttribute("normal"), mShader->GetAttribute("texCoord"));

	mDisplayTexture->UnSet(0);

	mShader->UnBind();
}

void Sample::Shutdown() {
	delete mShader;
	delete mDisplayTexture;
	delete mVertexPositions;
	delete mVertexNormals;
	delete mVertexTexCoords;
	delete mIndexBuffer;
}