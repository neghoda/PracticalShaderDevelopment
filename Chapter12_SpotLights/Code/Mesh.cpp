#include "Mesh.h"
#include "Draw.h"
#include "Transform.h"

Mesh::Mesh() {
	mPosAttrib = new Attribute<vec3>();
	mNormAttrib = new Attribute<vec3>();
	mUvAttrib = new Attribute<vec2>();
	mTangentAttrib = new Attribute<vec3>();
	mIndexBuffer = new IndexBuffer();
}

Mesh::Mesh(const Mesh& other) {
	mPosAttrib = new Attribute<vec3>();
	mNormAttrib = new Attribute<vec3>();
	mUvAttrib = new Attribute<vec2>();
	mTangentAttrib = new Attribute<vec3>();
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
	mTangent = other.mTangent;
	UpdateOpenGLBuffers();
	return *this;
}

Mesh::~Mesh() {
	delete mPosAttrib;
	delete mNormAttrib;
	delete mUvAttrib;
	delete mTangentAttrib;
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
	if (mTangent.size() > 0) {
		mTangentAttrib->Set(mTangent);
	}
}

void Mesh::Bind(int position, int normal, int texCoord, int tanget) {
	if (position >= 0) {
		mPosAttrib->BindTo(position);
	}
	if (normal >= 0) {
		mNormAttrib->BindTo(normal);
	}
	if (texCoord >= 0) {
		mUvAttrib->BindTo(texCoord);
	}
	if (tanget >= 0) {
		mTangentAttrib->BindTo(tanget);
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

void Mesh::UnBind(int position, int normal, int texCoord, int tanget) {
	if (position >= 0) {
		mPosAttrib->UnBindFrom(position);
	}
	if (normal >= 0) {
		mNormAttrib->UnBindFrom(normal);
	}
	if (texCoord >= 0) {
		mUvAttrib->UnBindFrom(texCoord);
	}
	if (tanget >= 0) {
		mTangentAttrib->UnBindFrom(tanget);
	}
}

void Mesh::CalculateTangents() {
	std::vector<vec3> tangents;
	tangents.resize(mPosition.size());

	for (int i = 0; i < mIndices.size() - 2; i += 3) {
		const vec3& v0 = mPosition[mIndices[i]];
		const vec3& v1 = mPosition[mIndices[i + 1]];
		const vec3& v2 = mPosition[mIndices[i + 2]];
		const vec2& uv0 = mTexCoord[mIndices[i]];
		const vec2& uv1 = mTexCoord[mIndices[i + 1]];
		const vec2& uv2 = mTexCoord[mIndices[i + 2]];

		vec3 edge1 = v1 - v0;
		vec3 edge2 = v2 - v0;
		vec2 dUV1 = vec2(uv1.x - uv0.x, uv1.y - uv0.y);
		vec2 dUV2 = vec2(uv2.x - uv0.x, uv2.y - uv0.y);

		float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

		vec3 tan;
		tan.x = f * (dUV2.y * edge1.x - dUV1.y * edge2.x);
		tan.y = f * (dUV2.y * edge1.y - dUV1.y * edge2.y);
		tan.z = f * (dUV2.y * edge1.z - dUV1.y * edge2.z);
		normalize(tan);

		tangents[mIndices[i]] = tangents[mIndices[i]] + tan;
		tangents[mIndices[i + 1]] = tangents[mIndices[i + 1]] + tan;
		tangents[mIndices[i + 2]] = tangents[mIndices[i + 2]] + tan;
	}

	for (int i = 0; i < tangents.size(); ++i) {
		normalize(tangents[i]);
	}

	mTangent = tangents;
}