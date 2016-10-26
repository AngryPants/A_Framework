#include "EntityTest.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"	

EntityTest::EntityTest(const string& name) : EntityBase(name) {

	mesh = MeshBuilder::GetInstance().GenerateQuad("Test Quad", Color(1, 1, 1), 1.0f);
	textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Texture", "Image//Texture_Test.tga");

}

EntityTest::~EntityTest() {
}

void EntityTest::Update(double deltaTime) {
}

void EntityTest::Render() {

	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(0, 1, 0, 0);
		RenderHelper::GetInstance().RenderMesh(*mesh, textureList, false);
	modelStack.PopMatrix();

}