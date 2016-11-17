#ifndef CUBE_SCRIPT_H
#define CUBE_SCRIPT_H

#include "../Script/Script.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"

class CubeScript : public Script {

public:
	GameObject* go;

	CubeScript() {
		go = nullptr;
	}
	virtual ~CubeScript() {}

	virtual void Update(double deltaTime) {
		if (go == nullptr) {
			return;
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LEFT]) {
			go->GetComponent<Transform>().Translate(5.0f * deltaTime, 0, 0);
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RIGHT]) {
			go->GetComponent<Transform>().Translate(-5.0f * deltaTime, 0, 0);
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_FORWARD]) {
			go->GetComponent<Transform>().Translate(0, 5.0f * deltaTime, 0);
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_BACKWARD]) {
			go->GetComponent<Transform>().Translate(0, -5.0f * deltaTime, 0);
		}
	}

};

#endif