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
	mShaderDirectLight = new Shader("Shaders/vertex.vert", "Shaders/dirLight.frag");
	mShaderPointLight = new Shader("Shaders/vertex.vert", "Shaders/pointLight.frag");
	mDisplayTexture = new Texture("Assets/shield_diffuse.png");
	mSpecularTexture = new Texture("Assets/shield_spec.png");
	mNormalTexture = new Texture("Assets/shield_normal.png");

	mWaterMesh = PLYHelpers::LoadPLYFile("Assets/plane.ply");
	mWaterMesh->CalculateTangents();
	mWaterMesh->UpdateOpenGLBuffers();

	mWaterShaderDirectLight = new Shader("Shaders/water.vert", "Shaders/dirLightWater.frag");
	mWaterShaderPointLight = new Shader("Shaders/water.vert", "Shaders/pointLightWater.frag");
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

	SetBlendModeAlpha();

	mShaderDirectLight->Bind();

	Uniform<mat4>::Set(mShaderDirectLight->GetUniform("model"), model);
	Uniform<mat4>::Set(mShaderDirectLight->GetUniform("view"), view);
	Uniform<mat4>::Set(mShaderDirectLight->GetUniform("projection"), projection);

	mDisplayTexture->Set(mShaderDirectLight->GetUniform("tex0"), 0);
	mSpecularTexture->Set(mShaderDirectLight->GetUniform("tex1"), 1);
	mNormalTexture->Set(mShaderDirectLight->GetUniform("tex2"), 2);
	mSkyboxCubemap->Set(mShaderDirectLight->GetUniform("tex3"), 3);

	mMesh->Bind(mShaderDirectLight->GetAttribute("position"), mShaderDirectLight->GetAttribute("normal"), mShaderDirectLight->GetAttribute("texCoord"), mShaderDirectLight->GetAttribute("tangent"));
	
	Uniform<vec3>::Set(mShaderDirectLight->GetUniform("light"), vec3(0, 0, -1));
	Uniform<vec4>::Set(mShaderDirectLight->GetUniform("lightColor"), vec4(1, 1, 1, 1));
	Uniform<vec4>::Set(mShaderDirectLight->GetUniform("ambientColor"), vec4(0, 0, 0, 1));
	Uniform<vec3>::Set(mShaderDirectLight->GetUniform("cameraPosition"), cameraPosition);

	mMesh->Draw();

	mMesh->UnBind(mShaderDirectLight->GetAttribute("position"), mShaderDirectLight->GetAttribute("normal"), mShaderDirectLight->GetAttribute("texCoord"), mShaderDirectLight->GetAttribute("tangent"));

	mDisplayTexture->UnSet(0);
	mSpecularTexture->UnSet(1);
	mNormalTexture->UnSet(2);
	mSkyboxCubemap->UnSet(3);

	mShaderDirectLight->UnBind();

	mShaderPointLight->Bind();

	Uniform<mat4>::Set(mShaderPointLight->GetUniform("model"), model);
	Uniform<mat4>::Set(mShaderPointLight->GetUniform("view"), view);
	Uniform<mat4>::Set(mShaderPointLight->GetUniform("projection"), projection);

	mDisplayTexture->Set(mShaderPointLight->GetUniform("tex0"), 0);
	mSpecularTexture->Set(mShaderPointLight->GetUniform("tex1"), 1);
	mNormalTexture->Set(mShaderPointLight->GetUniform("tex2"), 2);
	mSkyboxCubemap->Set(mShaderPointLight->GetUniform("tex3"), 3);

	mMesh->Bind(mShaderPointLight->GetAttribute("position"), mShaderPointLight->GetAttribute("normal"), mShaderPointLight->GetAttribute("texCoord"), mShaderPointLight->GetAttribute("tangent"));
	
	Uniform<vec3>::Set(mShaderPointLight->GetUniform("lightPos"), vec3(sin(mTime), 0, 0.25));
	Uniform<float>::Set(mShaderPointLight->GetUniform("lightRadius"), 5);
	Uniform<vec4>::Set(mShaderPointLight->GetUniform("lightColor"), vec4(1, 0, 0, 1));
	Uniform<vec3>::Set(mShaderPointLight->GetUniform("cameraPosition"), cameraPosition);

	mMesh->Draw();

	Uniform<vec3>::Set(mShaderPointLight->GetUniform("lightPos"), vec3(-sin(mTime), 0, 0.25));
	Uniform<float>::Set(mShaderPointLight->GetUniform("lightRadius"), 5);
	Uniform<vec4>::Set(mShaderPointLight->GetUniform("lightColor"), vec4(0, 1, 0, 1));
	Uniform<vec3>::Set(mShaderPointLight->GetUniform("cameraPosition"), cameraPosition);

	mMesh->Draw();

	mMesh->UnBind(mShaderPointLight->GetAttribute("position"), mShaderPointLight->GetAttribute("normal"), mShaderPointLight->GetAttribute("texCoord"), mShaderPointLight->GetAttribute("tangent"));

	mDisplayTexture->UnSet(0);
	mSpecularTexture->UnSet(1);
	mNormalTexture->UnSet(2);
	mSkyboxCubemap->UnSet(3);

	mShaderPointLight->UnBind();

	mWaterShaderDirectLight->Bind();

	Transform waterTransform = Transform();

	waterTransform.rotation = angleAxis(260 * DEG2RAD, vec3(1, 0, 0));
	waterTransform.position = vec3(0, -0.5, 0.5);
	waterTransform.scale = vec3(4, 4, 4);

	Uniform<mat4>::Set(mWaterShaderDirectLight->GetUniform("model"), transformToMat4(waterTransform));
	Uniform<mat4>::Set(mWaterShaderDirectLight->GetUniform("view"), view);
	Uniform<mat4>::Set(mWaterShaderDirectLight->GetUniform("projection"), projection);

	mWaterNormalTexture->Set(mWaterShaderDirectLight->GetUniform("tex0"), 0);
	mSkyboxCubemap->Set(mWaterShaderDirectLight->GetUniform("tex1"), 1);

	mWaterMesh->Bind(mWaterShaderDirectLight->GetAttribute("position"), mWaterShaderDirectLight->GetAttribute("normal"), mWaterShaderDirectLight->GetAttribute("texCoord"), mWaterShaderDirectLight->GetAttribute("tangent"));
	
	Uniform<vec3>::Set(mWaterShaderDirectLight->GetUniform("light"), vec3(0, 5, 0));
	Uniform<vec4>::Set(mWaterShaderDirectLight->GetUniform("lightColor"), vec4(1, 1, 1, 1));
	Uniform<vec4>::Set(mWaterShaderDirectLight->GetUniform("ambientColor"), vec4(0, 0, 0, 1));
	Uniform<vec3>::Set(mWaterShaderDirectLight->GetUniform("cameraPosition"), cameraPosition);
	Uniform<float>::Set(mWaterShaderDirectLight->GetUniform("time"), mTime);
	
	mWaterMesh->Draw();

	mWaterMesh->UnBind(mWaterShaderDirectLight->GetAttribute("position"), mWaterShaderDirectLight->GetAttribute("normal"), mWaterShaderDirectLight->GetAttribute("texCoord"), mWaterShaderDirectLight->GetAttribute("tangent"));

	mWaterNormalTexture->UnSet(0);
	mSkyboxCubemap->UnSet(1);

	mWaterShaderDirectLight->UnBind();

	mWaterShaderPointLight->Bind();

	Uniform<mat4>::Set(mWaterShaderPointLight->GetUniform("model"), transformToMat4(waterTransform));
	Uniform<mat4>::Set(mWaterShaderPointLight->GetUniform("view"), view);
	Uniform<mat4>::Set(mWaterShaderPointLight->GetUniform("projection"), projection);

	mWaterNormalTexture->Set(mWaterShaderPointLight->GetUniform("tex0"), 0);
	mSkyboxCubemap->Set(mWaterShaderPointLight->GetUniform("tex1"), 1);

	mWaterMesh->Bind(mWaterShaderPointLight->GetAttribute("position"), mWaterShaderPointLight->GetAttribute("normal"), mWaterShaderPointLight->GetAttribute("texCoord"), mWaterShaderPointLight->GetAttribute("tangent"));
	
	Uniform<vec3>::Set(mWaterShaderPointLight->GetUniform("lightPos"), vec3(sin(mTime), 0, 0.25));
	Uniform<float>::Set(mWaterShaderPointLight->GetUniform("lightRadius"), 5);
	Uniform<vec4>::Set(mWaterShaderPointLight->GetUniform("lightColor"), vec4(1, 0, 0, 1));
	Uniform<vec3>::Set(mWaterShaderPointLight->GetUniform("cameraPosition"), cameraPosition);
	Uniform<float>::Set(mWaterShaderPointLight->GetUniform("time"), mTime);

	mWaterMesh->Draw();

	Uniform<vec3>::Set(mWaterShaderPointLight->GetUniform("lightPos"), vec3(-sin(mTime), 0, 0.25));
	Uniform<float>::Set(mWaterShaderPointLight->GetUniform("lightRadius"), 5);
	Uniform<vec4>::Set(mWaterShaderPointLight->GetUniform("lightColor"), vec4(0, 1, 0, 1));
	Uniform<vec3>::Set(mWaterShaderPointLight->GetUniform("cameraPosition"), cameraPosition);
	Uniform<float>::Set(mWaterShaderPointLight->GetUniform("time"), mTime);

	mWaterMesh->Draw();

	mWaterMesh->UnBind(mWaterShaderPointLight->GetAttribute("position"), mWaterShaderPointLight->GetAttribute("normal"), mWaterShaderPointLight->GetAttribute("texCoord"), mWaterShaderPointLight->GetAttribute("tangent"));

	mWaterNormalTexture->UnSet(0);
	mSkyboxCubemap->UnSet(1);

	mWaterShaderPointLight->UnBind();

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
	delete mDisplayTexture;
	delete mVertexPositions;
	delete mVertexNormals;
	delete mVertexTexCoords;
	delete mIndexBuffer;
}