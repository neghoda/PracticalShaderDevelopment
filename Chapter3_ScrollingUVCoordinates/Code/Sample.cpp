#include "Sample.h"
#include "mat4.h"
#include "quat.h"
#include "Uniform.h"
#include "Draw.h"

void Sample::Initialize() {
	mRotation = 0.0f;
	mShader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");
	mDisplayTexture = new Texture("Assets/parrot.png");

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
	mTimeSinceStart += inDeltaTime;
}

void Sample::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 model = quatToMat4(angleAxis(0, vec3(0, 0, 1)));

	mShader->Bind();

	mVertexPositions->BindTo(mShader->GetAttribute("position"));
	mVertexTexCoords->BindTo(mShader->GetAttribute("texCoord"));

	Uniform<mat4>::Set(mShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mShader->GetUniform("projection"), projection);
	Uniform<float>::Set(mShader->GetUniform("time"), mTimeSinceStart);

	mDisplayTexture->Set(mShader->GetUniform("tex0"), 0);

	Draw(*mIndexBuffer, DrawMode::Triangles);

	mDisplayTexture->UnSet(0);

	mVertexPositions->UnBindFrom(mShader->GetAttribute("position"));
	mVertexTexCoords->UnBindFrom(mShader->GetAttribute("texCoord"));

	mShader->UnBind();
}

void Sample::Shutdown() {
	delete mShader;
	delete mDisplayTexture;
	delete mVertexPositions;
	delete mVertexTexCoords;
	delete mIndexBuffer;
}
