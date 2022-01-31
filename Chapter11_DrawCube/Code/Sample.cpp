#include "Sample.h"
#include "mat4.h"
#include "quat.h"
#include "Uniform.h"
#include "Draw.h"
#include "PLYHelpers.h"
#include "Cubemap.h"

void Sample::Initialize() {
	mRotation = 0.0f;
	mShader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");
	
	mMesh = PLYHelpers::LoadPLYFile("Assets/cube.ply");
	mMesh->UpdateOpenGLBuffers();

	mCubemap = new Cubemap("Assets/cube_front.jpg",
		"Assets/cube_back.jpg",
		"Assets/cube_right.jpg",
		"Assets/cube_left.jpg",
		"Assets/cube_top.jpg",
		"Assets/cube_bottom.jpg");
}

void Sample::Update(float inDeltaTime) {
	mRotation += inDeltaTime * 45.0f;
	while (mRotation > 360.0f) {
		mRotation -= 360.0f;
	}
}

void Sample::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 model = quatToMat4(angleAxis(mRotation * DEG2RAD, vec3(1, 1, 1)));

	mShader->Bind();

	Uniform<mat4>::Set(mShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mShader->GetUniform("projection"), projection);

	mCubemap->Set(mShader->GetUniform("tex0"), 0);

	mMesh->Bind(mShader->GetAttribute("position"), -1, -1, -1);
	mMesh->Draw();
	mMesh->UnBind(mShader->GetAttribute("position"), -1, -1, -1);

	mCubemap->UnSet(0);

	mShader->UnBind();
}

void Sample::Shutdown() {
	delete mShader;
	delete mMesh;
	delete mCubemap;
}