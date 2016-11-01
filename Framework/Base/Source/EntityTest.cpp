#include "EntityTest.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"	

EntityTest::EntityTest(const string& name) : EntityBase(name) {

	//mesh = MeshBuilder::GetInstance().GenerateQuad("Test Quad", Color(1, 1, 1), 1.0f);
	//mesh = MeshBuilder::GetInstance().GenerateOBJ("Test AxisStar", "OBJ\\Default\\AxisStar.obj");
	//mesh = MeshBuilder::GetInstance().GenerateOBJ("Test Cube", "OBJ\\Default\\Cube.obj");
	mesh = MeshBuilder::GetInstance().GenerateOBJ("Test Sphere", "OBJ\\Default\\Sphere.obj");
	//textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Texture", "Image//Default//AxisStar.tga");

}

EntityTest::~EntityTest() {
}

void EntityTest::Update(double deltaTime) {
}

void EntityTest::Render() {

	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	modelStack.PushMatrix();
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.Rotate(rotation.y, 0, 1, 0);
		modelStack.Rotate(rotation.x, 1, 0, 0);
		modelStack.Rotate(rotation.z, 0, 0, 1);
		RenderHelper::GetInstance().RenderMesh(*mesh, textureList, true);
	modelStack.PopMatrix();

}

void EntityTest::RenderUI() {
}