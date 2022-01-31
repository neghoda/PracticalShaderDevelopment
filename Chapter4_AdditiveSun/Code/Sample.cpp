#include "Sample.h"
#include "mat4.h"
#include "quat.h"
#include "Uniform.h"
#include "Draw.h"
#include "Transform.h"

void Sample::Initialize() {
	mRotation = 0.0f;
	
	mAlienShader = new Shader("Shaders/vertex.vert", "Shaders/alien.frag");
	mCloudShader = new Shader("Shaders/vertex.vert", "Shaders/cloud.frag");
	
	mAlienTexture = new Texture("Assets/alien.png");
	mBackgroundTexture = new Texture("Assets/forest.png");
	mCloudTexture = new Texture("Assets/cloud.png");
	mSunTexture = new Texture("Assets/sun.png");

	mVertexPositions = new Attribute<vec3>();
	mVertexTexCoords = new Attribute<vec2>();
	mIndexBuffer = new IndexBuffer();

	std::vector<vec3> positions;
	positions.push_back(vec3(-1, -1, 0));
	positions.push_back(vec3(-1, 1, 0));
	positions.push_back(vec3(1, -1, 0));
	positions.push_back(vec3(1, 1, 0));
	mVertexPositions->Set(positions);

	std::vector<vec2> uvs;
	uvs.push_back(vec2(0, 0));
	uvs.push_back(vec2(0, 1));
	uvs.push_back(vec2(1, 0));
	uvs.push_back(vec2(1, 1));
	mVertexTexCoords->Set(uvs);

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	mIndexBuffer->Set(indices);
}

void Sample::Update(float inDeltaTime) {
}

void Sample::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));
	quat rotation = angleAxis(180 * DEG2RAD, vec3(0, 0, 1));

	std::vector<mat4> models;

	Transform model = Transform();
	model.rotation = rotation;

	// Alien
	model.position = vec3(0, -0.8, 0);
	models.push_back(transformToMat4(model));

	// Background
	model.position = vec3(0, 0, 0);
	model.scale = vec3(4, 4, 4);
	models.push_back(transformToMat4(model));

	// Cloud
	model.position = vec3(2, 0.5, 0);
	model.scale = vec3(1, 0.5, 1);
	models.push_back(transformToMat4(model));

	// Sun
	model.position = vec3(0, 0, 0);
	model.scale = vec3(5, 5, 5);
	models.push_back(transformToMat4(model));
	
	Shader* mShader = mAlienShader; 
	EnableDepthTest();
	DisableBlend();
	mShader->Bind();

	mVertexPositions->BindTo(mShader->GetAttribute("position"));
	mVertexTexCoords->BindTo(mShader->GetAttribute("texCoord"));

	Uniform<mat4>::Set(mShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mShader->GetUniform("projection"), projection);

	mAlienTexture->Set(mShader->GetUniform("tex0"), 0);
	Uniform<mat4>::Set(mShader->GetUniform("model"), models[0]);
	Draw(*mIndexBuffer, DrawMode::Triangles);
	mAlienTexture->UnSet(0);
	
	mBackgroundTexture->Set(mShader->GetUniform("tex0"), 0);
	Uniform<mat4>::Set(mShader->GetUniform("model"), models[1]);
	Draw(*mIndexBuffer, DrawMode::Triangles);
	mBackgroundTexture->UnSet(0);

	mVertexPositions->UnBindFrom(mShader->GetAttribute("position"));
	mVertexTexCoords->UnBindFrom(mShader->GetAttribute("texCoord"));

	mShader->UnBind();
	mShader = mCloudShader;
	mShader->Bind();
	
	DisableDepthTest();
	SetBlendModeAlpha();

	mVertexPositions->BindTo(mShader->GetAttribute("position"));
	mVertexTexCoords->BindTo(mShader->GetAttribute("texCoord"));

	Uniform<mat4>::Set(mShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mShader->GetUniform("projection"), projection);
	
	mCloudTexture->Set(mShader->GetUniform("tex0"), 0);
	Uniform<mat4>::Set(mShader->GetUniform("model"), models[2]);
	Draw(*mIndexBuffer, DrawMode::Triangles);
	mCloudTexture->UnSet(0);
	
	SetBlendModeAdd();

	mSunTexture->Set(mShader->GetUniform("tex0"), 0);
	Uniform<mat4>::Set(mShader->GetUniform("model"), models[3]);
	Draw(*mIndexBuffer, DrawMode::Triangles);
	mSunTexture->UnSet(0);

	mVertexPositions->UnBindFrom(mShader->GetAttribute("position"));
	mVertexTexCoords->UnBindFrom(mShader->GetAttribute("texCoord"));

	mShader->UnBind();
}

void Sample::Shutdown() {
	delete mAlienShader;
	delete mCloudShader;
	delete mAlienTexture;
	delete mBackgroundTexture;
	delete mCloudTexture;
	delete mSunTexture;
	delete mVertexPositions;
	delete mVertexTexCoords;
	delete mIndexBuffer;
}
