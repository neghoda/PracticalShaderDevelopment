#include "Sample.h"
#include "mat4.h"
#include "quat.h"
#include "Uniform.h"
#include "Draw.h"
#include "PLYHelpers.h"
#include "Transform.h"
#include "Cubemap.h"

void Sample::Initialize() {
	mMesh = PLYHelpers::LoadPLYFile("Assets/shield.ply");
	mMesh->CalculateTangents();
	mMesh->UpdateOpenGLBuffers();

	mRotation = 0.0f;
	mShader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");
	mDisplayTexture = new Texture("Assets/shield_diffuse.png");
	mSpecularTexture = new Texture("Assets/shield_spec.png");
	mNormalTexture = new Texture("Assets/shield_normal.png");

	mWaterMesh = PLYHelpers::LoadPLYFile("Assets/plane.ply");
	mWaterMesh->CalculateTangents();
	mWaterMesh->UpdateOpenGLBuffers();

	mWaterShader = new Shader("Shaders/water.vert", "Shaders/water.frag");
	mWaterNormalTexture = new Texture("Assets/water_normal.png");

	mSkyboxShader = new Shader("Shaders/skybox.vert", "Shaders/skybox.frag");
	mSkyboxMesh = PLYHelpers::LoadPLYFile("Assets/cube.ply");
	mSkyboxMesh->CalculateTangents();
	mSkyboxMesh->UpdateOpenGLBuffers();
	mSkyboxCubemap = new Cubemap("Assets/night_front.jpg",
		"Assets/night_back.jpg",
		"Assets/night_right.jpg",
		"Assets/night_left.jpg",
		"Assets/night_top.jpg",
		"Assets/night_bottom.jpg");
}

void Sample::Update(float inDeltaTime) {
	mRotation += inDeltaTime * 45.0f;
	while (mRotation > 360.0f) {
		mRotation -= 360.0f;
	}

	mTime += inDeltaTime;
}

void Sample::Render(float inAspectRatio) {
	vec3 cameraPosition = vec3(0, 0, -1);
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(cameraPosition, vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 model = quatToMat4(angleAxis(mRotation * DEG2RAD, vec3(1, 0, 0)));

	mShader->Bind();

	Uniform<mat4>::Set(mShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mShader->GetUniform("projection"), projection);
	Uniform<vec3>::Set(mShader->GetUniform("lightPos"), vec3(0, sin(mTime), 0.25));
	Uniform<float>::Set(mShader->GetUniform("lightRadius"), 1);
	Uniform<vec4>::Set(mShader->GetUniform("lightColor"), vec4(1, 1, 1, 1));
	Uniform<vec4>::Set(mShader->GetUniform("ambientColor"), vec4(1, 1, 1, 1));
	Uniform<vec3>::Set(mShader->GetUniform("cameraPosition"), cameraPosition);

	mDisplayTexture->Set(mShader->GetUniform("tex0"), 0);
	mSpecularTexture->Set(mShader->GetUniform("tex1"), 1);
	mNormalTexture->Set(mShader->GetUniform("tex2"), 2);
	mSkyboxCubemap->Set(mShader->GetUniform("tex3"), 3);

	mMesh->Bind(mShader->GetAttribute("position"), mShader->GetAttribute("normal"), mShader->GetAttribute("texCoord"), mShader->GetAttribute("tangent"));
	mMesh->Draw();
	mMesh->UnBind(mShader->GetAttribute("position"), mShader->GetAttribute("normal"), mShader->GetAttribute("texCoord"), mShader->GetAttribute("tangent"));

	mDisplayTexture->UnSet(0);
	mSpecularTexture->UnSet(1);
	mNormalTexture->UnSet(2);
	mSkyboxCubemap->UnSet(3);

	mShader->UnBind();

	mWaterShader->Bind();

	Transform waterTransform = Transform();

	waterTransform.rotation = angleAxis(260 * DEG2RAD, vec3(1, 0, 0));
	waterTransform.position = vec3(0, -0.5, 0.5);
	waterTransform.scale = vec3(4, 4, 4);

	Uniform<mat4>::Set(mWaterShader->GetUniform("model"), transformToMat4(waterTransform));
	Uniform<mat4>::Set(mWaterShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mWaterShader->GetUniform("projection"), projection);
	Uniform<vec3>::Set(mWaterShader->GetUniform("lightPos"), vec3(sin(mTime), 0, 0.25));
	Uniform<float>::Set(mWaterShader->GetUniform("lightRadius"), 1);
	Uniform<vec4>::Set(mWaterShader->GetUniform("lightColor"), vec4(1, 1, 1, 1));
	Uniform<vec4>::Set(mWaterShader->GetUniform("ambientColor"), vec4(0.1, 0.1, 0.1, 1));
	Uniform<vec3>::Set(mWaterShader->GetUniform("cameraPosition"), cameraPosition);
	Uniform<float>::Set(mWaterShader->GetUniform("time"), mTime);

	mWaterNormalTexture->Set(mWaterShader->GetUniform("tex0"), 0);
	mSkyboxCubemap->Set(mWaterShader->GetUniform("tex1"), 1);

	mWaterMesh->Bind(mWaterShader->GetAttribute("position"), mWaterShader->GetAttribute("normal"), mWaterShader->GetAttribute("texCoord"), mWaterShader->GetAttribute("tangent"));
	mWaterMesh->Draw();
	mWaterMesh->UnBind(mWaterShader->GetAttribute("position"), mWaterShader->GetAttribute("normal"), mWaterShader->GetAttribute("texCoord"), mWaterShader->GetAttribute("tangent"));

	mWaterNormalTexture->UnSet(0);
	mSkyboxCubemap->UnSet(1);

	mWaterShader->UnBind();

	mSkyboxShader->Bind();
	mSkyboxShader->EnableFrontFaceCulling();

	Uniform<mat4>::Set(mSkyboxShader->GetUniform("model"), mat4());
	Uniform<mat4>::Set(mSkyboxShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mSkyboxShader->GetUniform("projection"), projection);

	mSkyboxCubemap->Set(mSkyboxShader->GetUniform("tex0"), 0);

	mSkyboxMesh->Bind(mSkyboxShader->GetAttribute("position"), -1, -1, -1);
	mSkyboxMesh->Draw();
	mSkyboxMesh->UnBind(mSkyboxShader->GetAttribute("position"), -1, -1, -1);

	mSkyboxCubemap->UnSet(0);

	mSkyboxShader->EnableBackFaceCulling();
	mSkyboxShader->UnBind();
}

void Sample::Shutdown() {
	delete mShader;
	delete mDisplayTexture;
	delete mVertexPositions;
	delete mVertexNormals;
	delete mVertexTexCoords;
	delete mIndexBuffer;
}