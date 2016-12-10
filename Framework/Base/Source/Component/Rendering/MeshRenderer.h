#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "../Component.h"
#include "../../Mesh/Mesh.h"
#include "../../Texture/TextureList.h"

class MeshRenderer : public Component {

public:
	//Variable(s)
	bool lightEnabled;
	
	//Constructor(s) & Destructor
	MeshRenderer(GameObject& gameObject);
	virtual ~MeshRenderer();

};

#endif