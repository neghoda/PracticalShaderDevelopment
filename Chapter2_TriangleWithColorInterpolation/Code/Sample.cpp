#include "Sample.h"
#include "Draw.h"

void Sample::Initialize() {
	mShader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");

	mVertexPositions = new Attribute<vec3>();
	mVertexColors = new Attribute<vec3>();

	std::vector<vec3> positions;
	std::vector<vec3> colors;

	positions.push_back(vec3(-1, 1, 0));
	positions.push_back(vec3(-1, -1, 0));
	positions.push_back(vec3(1, -1, 0));
	mVertexPositions->Set(positions);

	colors.push_back(vec3(1, 0, 0));
	colors.push_back(vec3(0, 1, 0));
	colors.push_back(vec3(0, 0, 1));
	mVertexColors->Set(colors);
}

void Sample::Update(float inDeltaTime) {}

void Sample::Render(float inAspectRatio) {
	mShader->Bind();

	mVertexPositions->BindTo(mShader->GetAttribute("position"));
	mVertexColors->BindTo(mShader->GetAttribute("color"));

	Draw(mVertexPositions->Count(), DrawMode::Triangles);

	mVertexPositions->UnBindFrom(mShader->GetAttribute("position"));
	mVertexColors->UnBindFrom(mShader->GetAttribute("color"));

	mShader->UnBind();
}

void Sample::Shutdown() {
	delete mShader;
	delete mVertexPositions;
	delete mVertexColors;
}
