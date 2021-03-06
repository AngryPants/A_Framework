#ifndef MESH_HOLDER_H
#define MESH_HOLDER_H

#include "../Component.h"
#include "../../Mesh/Mesh.h"
#include "../../Texture/TextureList.h"

class MeshHolder : public Component {

public:
	//Variable(s)
	Mesh* mesh;
	TextureList textureList;
	
	//Constructor(s) & Destructor
	MeshHolder(GameObject& _gameObject, PassKey<ComponentManager> _key);
	virtual ~MeshHolder();

	//Mesh
	bool SetMesh(Mesh* _mesh);
	bool RemoveMesh();
	Mesh* GetMesh();

	//Texture
	bool SetTextures(const TextureList& _textureList);
	bool RemoveTextureList();
	TextureList& GetTextureList();

};

#endif