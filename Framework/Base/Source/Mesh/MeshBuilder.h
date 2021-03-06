#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "SingletonTemplate.h"
#include "../Animation/SpriteAnimation.h"
#include "Vertex.h"
#include <map>
#include <string>

using std::map;
using std::string;

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/

typedef map<string, Mesh*> MeshMap;
typedef map<string, SpriteAnimation*> SpriteMap;

class MeshBuilder : public Singleton<MeshBuilder> {

	friend class Singleton<MeshBuilder>;

private:
	MeshMap meshMap;
	SpriteMap spriteMap;

	//Constructor(s) & Destructor
	MeshBuilder();
	virtual ~MeshBuilder();

public:
	void Clear();
	bool CheckMeshExist(const string& meshName) const;
	bool CheckSpriteAnimationExist(const string& spriteName) const;
	Mesh* GetMesh(const string& meshName);
	SpriteAnimation* GetSpriteAnimation(const string& spriteName);

	Mesh* GenerateAxes(const std::string &meshName);
	Mesh* GenerateQuad(const std::string &meshName, Color color = Color(1, 1, 1), float length = 1.0f);
	Mesh* GeneratePlane(const std::string &meshName, Color color, float length = 1.0f, int uRepeat = 1, int vRepeat = 1);
	Mesh* GenerateCube(const std::string &meshName, Color color, float length = 1.0f);	
	Mesh* GenerateWireframeCube(const std::string &meshName, Color color, float length = 1.0f);	
	Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR = 1.0f, float innerR = 0.5f);
	Mesh* GenerateCircle(const string& meshName, Color color, unsigned int numSlice, float radius = 0.5f);
	Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 0.5f);
	Mesh* GenerateWireframeSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 0.5f);
	Mesh* GenerateSkyPlane(const std::string &meshName, Color color, unsigned int numSlices, float planetRadius, float atmosphereRadius, float uTile, float vTile);
	Mesh* GenerateOBJ(const std::string &meshName, const std::string& filePath);
	Mesh* GenerateText(const std::string &meshName, unsigned row, unsigned col);
	Mesh* GenerateTerrain(const std::string &meshName, const std::string filePath, std::vector<unsigned char> &heightMap, unsigned int uTile, unsigned int vTile);
	SpriteAnimation* GenerateSpriteAnimation(const string& meshName, unsigned int numRow, unsigned int numColumn);

};

#endif