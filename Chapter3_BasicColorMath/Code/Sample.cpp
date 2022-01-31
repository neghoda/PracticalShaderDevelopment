#include "Sample.h"
#include "mat4.h"
#include "quat.h"
#include "Uniform.h"
#include "Draw.h"
#include "Transform.h"

void Sample::Initialize() {
	mRotation = 0.0f;
	mMultiply = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mAdd = vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
}

void Sample::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));
	quat rotation = angleAxis(180 * DEG2RAD, vec3(0, 0, 1));

	std::vector<mat4> models;

	Transform model = Transform();
	model.rotation = rotation;
	
	// Upper Left
	model.position = vec3(1, 1, 0);
	models.push_back(transformToMat4(model));

	// Upper Right
	model.position = vec3(-1, 1, 0);
	models.push_back(transformToMat4(model));
	
	// Bottom Left
	model.position = vec3(-1, -1, 0);
	models.push_back(transformToMat4(model));
	
	// Bottom Right
	model.position = vec3(1, -1, 0);
	models.push_back(transformToMat4(model));
	
	mShader->Bind();

	mVertexPositions->BindTo(mShader->GetAttribute("position"));
	mVertexTexCoords->BindTo(mShader->GetAttribute("texCoord"));

	Uniform<vec4>::Set(mShader->GetUniform("multiply"), mMultiply);
	Uniform<mat4>::Set(mShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mShader->GetUniform("projection"), projection);

	mDisplayTexture->Set(mShader->GetUniform("tex0"), 0);

	mAdd = vec4(-0.5f, -0.5f, -0.5f, 1.0f); // flat grey subtraction
	Uniform<vec4>::Set(mShader->GetUniform("add"), mAdd);
	Uniform<mat4>::Set(mShader->GetUniform("model"), models[0]);
	Draw(*mIndexBuffer, DrawMode::Triangles);

	mAdd = vec4(0.25f, 0.25f, 1.0f, 1.0f); // blue addition
	Uniform<vec4>::Set(mShader->GetUniform("add"), mAdd);
	Uniform<mat4>::Set(mShader->GetUniform("model"), models[1]);
	Draw(*mIndexBuffer, DrawMode::Triangles);
	
	mAdd = vec4(1.0f, 0.0f, 0.0f, 1.0f); // red addition
	Uniform<vec4>::Set(mShader->GetUniform("add"), mAdd);
	Uniform<mat4>::Set(mShader->GetUniform("model"), models[2]);
	Draw(*mIndexBuffer, DrawMode::Triangles);
	
	mAdd = vec4(0.5f, 0.5f, 0.5f, 1.0f); // flat grey addition
	Uniform<vec4>::Set(mShader->GetUniform("add"), mAdd);
	Uniform<mat4>::Set(mShader->GetUniform("model"), models[3]);
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
