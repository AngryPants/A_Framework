#ifndef CUBE_SCRIPT_H
#define CUBE_SCRIPT_H

#include "../Script/Script.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"

class CubeScript : public Script {

public:
	CubeScript(GameObject& gameObject) : Script(gameObject) {}
	virtual ~CubeScript() {}

	virtual void Update(double deltaTime) {
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LEFT]) {
			GetGameObject().GetComponent<Transform>().Translate(5.0f * deltaTime, 0, 0);
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RIGHT]) {
			GetGameObject().GetComponent<Transform>().Translate(-5.0f * deltaTime, 0, 0);
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_FORWARD]) {
			GetGameObject().GetComponent<Transform>().Translate(0, 5.0f * deltaTime, 0);
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_BACKWARD]) {
			GetGameObject().GetComponent<Transform>().Translate(0, -5.0f * deltaTime, 0);
		}
	}

};

#endif