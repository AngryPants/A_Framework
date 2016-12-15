#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "../Scene/Scene.h"
#include "../Graphics/RenderHelper.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Others/Skybox.h"

class SceneTest : public Scene {

private:
	//Variable(s)
	GameObject* player;
	Skybox skybox;

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
	float debugCountdown = 3.0f;
};

#endif