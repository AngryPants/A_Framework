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
	MeshHolder(GameObject& _gameObject);
	virtual ~MeshHolder();

	//Mesh
	bool SetMesh(Mesh* _mesh);
	bool RemoveMesh();
	const Mesh* GetMesh() const;

	//Texture
	bool SetTextures(const TextureList& _textureList);
	bool RemoveTextureList();
	const TextureList& GetTextureList() const;

};

#endif