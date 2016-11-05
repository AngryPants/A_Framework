#ifndef SCENE_EMPTY_H
#define SCENE_EMPTY_H

#include "Scene.h"
#include "InputManager.h"

class SceneEmpty : public Scene {

public:
	//Constructor(s) & Destructor
	SceneEmpty(const string& name) : Scene(name) {}
	virtual ~SceneEmpty() {}

	//Virtual Function(s)
	virtual void Init() {}
	virtual void Save() {}
	virtual void Load() {}
	virtual void Update(double deltaTime) {
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
			Application::GetInstance().Quit();
		}
	}
	virtual void Render() {}
	virtual void Exit() {}

};

#endif