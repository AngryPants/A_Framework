#include "PlayerCameraScript.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"

//Constructor(s) & Destructor
PlayerCameraScript::PlayerCameraScript(GameObject& gameObject) : Script(gameObject) {
	rotationSpeed = 60.0f;
}

PlayerCameraScript::~PlayerCameraScript() {
}

//Interface Function(s)
void PlayerCameraScript::Update(double deltaTime) {
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_UP]) {
		GetGameObject().GetComponent<Transform>().Rotate(-deltaTime * rotationSpeed, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_DOWN]) {
		GetGameObject().GetComponent<Transform>().Rotate(deltaTime * rotationSpeed, 0, 0);
	}

	if (GetGameObject().GetComponent<Transform>().GetLocalRotation().x > 90.0f) {
		GetGameObject().GetComponent<Transform>().SetLocalRotationX(90.0f);
	} else if (GetGameObject().GetComponent<Transform>().GetLocalRotation().x < -90.0f) {
		GetGameObject().GetComponent<Transform>().SetLocalRotationX(-90.0f);
	}
}