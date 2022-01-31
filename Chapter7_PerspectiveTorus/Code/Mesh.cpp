#include "Mesh.h"
#include "Draw.h"
#include "Transform.h"

Mesh::Mesh() {
	mPosAttrib = new Attribute<vec3>();
	mNormAttrib = new Attribute<vec3>();
	mUvAttrib = new Attribute<vec2>();
	mIndexBuffer = new IndexBuffer();
}

Mesh::Mesh(const Mesh& other) {
	mPosAttrib = new Attribute<vec3>();
	mNormAttrib = new Attribute<vec3>();
	mUvAttrib = new Attribute<vec2>();
	mIndexBuffer = new IndexBuffer();
	*this = other;
}

Mesh& Mesh::operator=(const Mesh& other) {
	if (this == &other) {
		return *this;
	}
	mPosition = other.mPosition;
	mNormal = other.mNormal;
	mTexCoord = other.mTexCoord;
	mIndices = other.mIndices;
	UpdateOpenGLBuffers();
	return *this;
}

Mesh::~Mesh() {
	delete mPosAttrib;
	delete mNormAttrib;
	delete mUvAttrib;
	delete mIndexBuffer;
}

std::vector<vec3>& Mesh::GetPosition() {
	return mPosition;
}

std::vector<vec3>& Mesh::GetNormal() {
	return mNormal;
}

std::vector<vec2>& Mesh::GetTexCoord() {
	return mTexCoord;
}

std::vector<unsigned int>& Mesh::GetIndices() {
	return mIndices;
}

void Mesh::UpdateOpenGLBuffers() {
	if (mPosition.size() > 0) {
		mPosAttrib->Set(mPosition);
	}
	if (mNormal.size() > 0) {
		mNormAttrib->Set(mNormal);
	}
	if (mTexCoord.size() > 0) {
		mUvAttrib->Set(mTexCoord);
	}
	if (mIndices.size() > 0) {
		mIndexBuffer->Set(mIndices);
	}
}

void Mesh::Bind(int position, int normal, int texCoord) {
	if (position >= 0) {
		mPosAttrib->BindTo(position);
	}
	if (normal >= 0) {
		mNormAttrib->BindTo(normal);
	}
	if (texCoord >= 0) {
		mUvAttrib->BindTo(texCoord);
	}
}

void Mesh::Draw() {
	if (mIndices.size() > 0) {
		::Draw(*mIndexBuffer, DrawMode::Triangles);
	}
	else {
		::Draw(mPosition.size(), DrawMode::Triangles);
	}
}

void Mesh::DrawInstanced(unsigned int numInstances) {
	if (mIndices.size() > 0) {
		::DrawInstanced(*mIndexBuffer, DrawMode::Triangles, numInstances);
	}
	else {
		::DrawInstanced(mPosition.size(), DrawMode::Triangles, numInstances);
	}
}

void Mesh::UnBind(int position, int normal, int texCoord) {
	if (position >= 0) {
		mPosAttrib->UnBindFrom(position);
	}
	if (normal >= 0) {
		mNormAttrib->UnBindFrom(normal);
	}
	if (texCoord >= 0) {
		mUvAttrib->UnBindFrom(texCoord);
	}
}