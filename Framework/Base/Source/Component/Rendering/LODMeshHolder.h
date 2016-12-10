#ifndef LOD_MESH_HOLDER_H
#define LOD_MESH_HOLDER_H

#include "../Component.h"
#include "../../Mesh/Mesh.h"
#include "../../Texture/TextureList.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../GameObject/GameObject.h"

class LODMeshHolder : public Component {

public:
	enum DETAIL_LEVEL
	{
		LOW_DETAILS,		
		MID_DETAILS,
		HIGH_DETAILS,
		NUM_DETAIL_LEVEL,
	};

	//Constructor(s) & Destructor
	LODMeshHolder(GameObject& _gameObject);
	virtual ~LODMeshHolder();

	//Mesh
	bool SetLODMesh(Mesh* _meshLow, Mesh* _meshMid, Mesh* _meshHigh);
	bool SetLODMesh(Mesh* _mesh, const DETAIL_LEVEL _detailLevel);
	bool RemoveMesh(const DETAIL_LEVEL _detailLevel);
	bool RemoveAllMeshes();
	const Mesh* GetLODMesh(const DETAIL_LEVEL _detailLevel) const;

	//Textures
	bool SetLODTextures(const TextureList& _textureListLow, const TextureList& _textureListMid, const TextureList& _textureListHigh);
	bool SetLODTextures(const TextureList& _textureList, const DETAIL_LEVEL _detailLevel);
	bool RemoveTextures(const DETAIL_LEVEL _detailLevel);
	bool RemoveAllTextures();
	const TextureList& GetTextureList(const DETAIL_LEVEL _detailLevel) const;

//private:
	//Variable(s)
	Mesh* mesh[NUM_DETAIL_LEVEL];
	TextureList textureList[NUM_DETAIL_LEVEL];

};

#endif