#include "Sample.h"
#include "Draw.h"
#include "Uniform.h"

void Sample::Initialize() {
	mShader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");

	mVertexPositions = new Attribute<vec3>();
	triangleColor = vec4(0, 1, 1, 1);

	std::vector<vec3> positions;

	positions.push_back(vec3(-1, 1, 0));
	positions.push_back(vec3(-1, -1, 0));
	positions.push_back(vec3(1, -1, 0));
	mVertexPositions->Set(positions);
}

void Sample::Update(float inDeltaTime) {}

void Sample::Render(float inAspectRatio) {
	mShader->Bind();

	mVertexPositions->BindTo(mShader->GetAttribute("position"));
	Uniform<vec4>::Set(mShader->GetUniform("triangleColor"), triangleColor);

	Draw(mVertexPositions->Count(), DrawMode::Triangles);

	mVertexPositions->UnBindFrom(mShader->GetAttribute("position"));

	mShader->UnBind();
}

void Sample::Shutdown() {
	delete mShader;
	delete mVertexPositions;
}
