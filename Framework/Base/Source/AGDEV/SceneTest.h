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

protected:
	//Virtual Function(s)
	virtual void Init();
	virtual void Update(double _deltaTime);
	virtual void Render();
	virtual void Exit();

public:
	//Constructor(s) & Destructor
	SceneTest(const string& name);
	virtual ~SceneTest();

	//Temporary Stuff
	float debugCountdown = 3.0f;
};

#endif