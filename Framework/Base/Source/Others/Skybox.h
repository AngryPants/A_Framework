#ifndef SKYBOX_H
#define SKYBOX_H

#include "../Mesh/MeshBuilder.h"
#include "../Texture/TextureList.h"

class Skybox {
private:
	//Variable(s)
	Mesh* mesh;

public:
	//Enum(s)
	enum TEXTURES {
		SKYBOX_BACK,
		SKYBOX_LEFT,
		SKYBOX_FRONT,
		SKYBOX_RIGHT,
		SKYBOX_TOP,
		SKYBOX_BOTTOM,
		NUM_SKYBOX_TEXTURES,
	};

	//Variable(s)
	TextureList textureLists[NUM_SKYBOX_TEXTURES];	

	//Constructor(s) & Destructor
	Skybox();
	virtual ~Skybox();

	//Function(s)
	Mesh* GetMesh() const;

};

#endif