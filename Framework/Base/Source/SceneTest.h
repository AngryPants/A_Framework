#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "Scene.h"
#include "RenderHelper.h"
#include "GameObjectFactory.h"

class SceneTest : public Scene {

private:
	GameObject* camera;
	GameObject* light;
	GameObject* cube;

public:
	//Constructor(s) & Destructor
	SceneTest(const string& name);
	virtual ~SceneTest();
	
	//Virtual Function(s)
	virtual void Init();
	virtual void Save() {}
	virtual void Load() {}
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void Exit();

};

#endif