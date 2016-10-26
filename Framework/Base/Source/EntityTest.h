#ifndef ENTITY_TEST_H
#define ENTITY_TEST_H

#include "EntityBase.h"
#include "MeshBuilder.h"
#include "TextureManager.h"
#include "TextureList.h"

class EntityTest : public EntityBase {

private:
	TextureList textureList;
	Mesh* mesh;

public:
	EntityTest(const string& name);
	virtual ~EntityTest();

	virtual void Update(double deltaTime);
	virtual void Render();

};

#endif