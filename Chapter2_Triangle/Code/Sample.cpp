#include "Sample.h"
#include "Draw.h"

void Sample::Initialize() {
	mShader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");

	mVertexPositions = new Attribute<vec3>();

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

	Draw(mVertexPositions->Count(), DrawMode::Triangles);

	mVertexPositions->UnBindFrom(mShader->GetAttribute("position"));

	mShader->UnBind();
}

void Sample::Shutdown() {
	delete mShader;
	delete mVertexPositions;
}
