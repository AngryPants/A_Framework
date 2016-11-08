#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Component.h"
#include "Mesh.h"
#include "TextureList.h"

class MeshRenderer : public Component {

public:
	//Variable(s)
	Mesh* mesh;
	TextureList textureList;
	bool lightEnabled;

	//Constructor(s) & Destructor
	MeshRenderer(GameObject& gameObject) : Component("Mesh Renderer", gameObject) {
		mesh = nullptr;
		lightEnabled = true;
	}
	virtual ~MeshRenderer() {}

};

#endif