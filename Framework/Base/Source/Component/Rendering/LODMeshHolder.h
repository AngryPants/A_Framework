#ifndef LOD_MESH_HOLDER_H
#define LOD_MESH_HOLDER_H

#include "../Component.h"
#include "MeshRenderer.h"
#include "../../Mesh/Mesh.h"
#include "../../Texture/TextureList.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../GameObject/GameObject.h"

class LODMeshHolder : public Component {

public:
	enum DETAIL_LEVEL
	{
		NO_DETAILS = 0,
		HIGH_DETAILS,
		MID_DETAILS,
		LOW_DETAILS,
		NUM_DETAIL_LEVEL,
	};

	//Constructor(s) & Destructor
	LODMeshHolder(GameObject& gameObject) : Component("LOD Mesh Holder", gameObject) 
	{
		if (GetGameObject().HasComponent<MeshRenderer>())
			GetGameObject().RemoveComponent<MeshRenderer>();
		
		for (int i = 0; i < NUM_DETAIL_LEVEL; i++)
			mesh[i] = nullptr;
		lightEnabled = true;
	}
	virtual ~LODMeshHolder() {}

	//functions
	bool InitLOD(Mesh* meshName_High, Mesh* meshName_Mid, Mesh* meshName_Low);

	bool SetLODMesh(Mesh* theMesh, const DETAIL_LEVEL theDetailLevel);
	Mesh* GetLODMesh(const DETAIL_LEVEL theDetailLevel) const;
	/*bool SetDetailLevel(const DETAIL_LEVEL theDetailLevel);*/

	//light
	bool lightEnabled;
private:
	//Variable(s)
	Mesh* mesh[NUM_DETAIL_LEVEL];
	TextureList textureList[NUM_DETAIL_LEVEL];
	//DETAIL_LEVEL theDetailLevel;
};

#endif