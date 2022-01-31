#ifndef _H_MESH_
#define _H_MESH_

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include <vector>
#include "Attribute.h"
#include "IndexBuffer.h"

class Mesh {
protected:
	std::vector<vec3> mPosition;
	std::vector<vec3> mNormal;
	std::vector<vec2> mTexCoord;
	std::vector<vec3> mTangent;
	std::vector<unsigned int> mIndices;
protected:
	Attribute<vec3>* mPosAttrib;
	Attribute<vec3>* mNormAttrib;
	Attribute<vec2>* mUvAttrib;
	Attribute<vec3>* mTangentAttrib;
	IndexBuffer* mIndexBuffer;
public:
	Mesh();
	Mesh(const Mesh&);
	Mesh& operator=(const Mesh&);
	~Mesh();
	std::vector<vec3>& GetPosition();
	std::vector<vec3>& GetNormal();
	std::vector<vec2>& GetTexCoord();
	std::vector<unsigned int>& GetIndices();
	void CalculateTangents();
	void UpdateOpenGLBuffers();
	void Bind(int position, int normal, int texCoord, int tanget);
	void Draw();
	void DrawInstanced(unsigned int numInstances);
	void UnBind(int position, int normal, int texCoord, int tanget);
};


#endif // !_H_MESH_
