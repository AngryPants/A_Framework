#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "../Scene/Scene.h"
#include "../Graphics/RenderHelper.h"
#include "../GameObject/GameObjectFactory.h"

class SceneTest : public Scene {

private:
	//Variable(s)
	GameObject* player;

public:
	//Constructor(s) & Destructor
	SceneTest(const string& name);
	virtual ~SceneTest();
	
	//Virtual Function(s)
	virtual void Init();
	virtual void Save() {}
	virtual void Load() {}
	virtual void Update(double _deltaTime);
	virtual void Render();
	virtual void Exit();

	//Temporary Stuff
	float debugCountdown = 0.0f;

};

#endif