#ifndef _H_PLY_HELPERS_
#define _H_PLY_HELPERS_

#include <vector>
#include <cassert>
#include "Mesh.h"
#include "happly.h"

namespace PLYHelpers {
	Mesh* LoadPLYFile(const char* path) {
		happly::PLYData plyIn(path);

		std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
		std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();

		std::vector<double> nxPos = plyIn.getElement("vertex").getProperty<double>("nx");
		std::vector<double> nyPos = plyIn.getElement("vertex").getProperty<double>("ny");
		std::vector<double> nzPos = plyIn.getElement("vertex").getProperty<double>("nz");

		std::vector<std::array<double, 3>> nPos(nxPos.size());
		for (size_t i = 0; i < nPos.size(); i++) {
			nPos[i][0] = nxPos[i];
			nPos[i][1] = nyPos[i];
			nPos[i][2] = nzPos[i];
		}

		std::vector<double> sPos = plyIn.getElement("vertex").getProperty<double>("s");
		std::vector<double> tPos = plyIn.getElement("vertex").getProperty<double>("t");

		std::vector<std::array<double, 2>> uvPos(sPos.size());
		for (size_t i = 0; i < uvPos.size(); i++) {
			uvPos[i][0] = sPos[i];
			uvPos[i][1] = tPos[i];
		}

		assert(vPos.size() == nPos.size() && nPos.size() == uvPos.size());

		Mesh* outMesh = new Mesh();

		std::vector<vec3>& positions = outMesh->GetPosition();
		std::vector<vec3>& normals = outMesh->GetNormal();
		std::vector<vec2>& texCoords = outMesh->GetTexCoord();
		std::vector<unsigned int>& indices = outMesh->GetIndices();

		for (int i = 0; i < vPos.size(); i++) {
			assert(vPos[i].size() == 3);
			assert(nPos[i].size() == 3);
			assert(uvPos[i].size() == 2);

			positions.push_back(vec3(vPos[i][0], vPos[i][1], vPos[i][2]));
			normals.push_back(vec3(nPos[i][0], nPos[i][1], nPos[i][2]));
			texCoords.push_back(vec2(uvPos[i][0], uvPos[i][1]));
		}

		for (int i = 0; i < fInd.size(); i++) {
			assert(fInd[i].size() == 3);
		
			indices.push_back(fInd[i][0]);
			indices.push_back(fInd[i][1]);
			indices.push_back(fInd[i][2]);
		}

		return outMesh;
	}
}

#endif 