#ifndef SCENE_SPLASH_H
#define SCENE_SPLASH_H

#include "../Scene/Scene.h"
#include "../Graphics/RenderHelper.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Others/Skybox.h"

class SceneSplash : public Scene {

private:
	float splashTimer;

protected:
	//Virtual Function(s)
	virtual void Init();
	virtual void Update(double _deltaTime);
	virtual void Render();
	virtual void Exit();

public:
	//Constructor(s) & Destructor
	SceneSplash(const string& _name);
	virtual ~SceneSplash();

	//Temporary Stuff
	float debugCountdown = 3.0f;
};

#endif