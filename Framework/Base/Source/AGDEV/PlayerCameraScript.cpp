#include "PlayerCameraScript.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Rendering/Camera.h"

//Constructor(s) & Destructor
PlayerCameraScript::PlayerCameraScript(GameObject& gameObject) : Script(gameObject) {
	rotationSpeed = 80.0f;
	fov = GetGameObject().GetComponent<Camera>().GetFOV();
}

PlayerCameraScript::~PlayerCameraScript() {
}

//Interface Function(s)
void PlayerCameraScript::Update(double deltaTime) {	
	float rotation = InputManager::GetInstance().GetInputInfo().axis[IAXIS_LOOK_VERTICAL];
	if (rotation != 0.0f) {
		GetGameObject().GetComponent<Transform>().Rotate(rotationSpeed * rotation, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_UP]) {
		GetGameObject().GetComponent<Transform>().Rotate(-deltaTime * rotationSpeed, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_DOWN]) {
		GetGameObject().GetComponent<Transform>().Rotate(deltaTime * rotationSpeed, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_ZOOM]) {
		fov = Math::Max(5.0f, fov - static_cast<float>(deltaTime) * 360.0f);
	} else {
		fov = Math::Min(60.0f, fov + static_cast<float>(deltaTime) * 360.0f);
	}

	Transform& transform = GetGameObject().GetComponent<Transform>();
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_CROUCH]) {
		if (transform.GetLocalPosition().y > 0.6f) {
			transform.Translate(0, -deltaTime* 5.0f, 0);
			if (transform.GetLocalPosition().y < 0.6f) {
				transform.SetLocalPosition(0, 0.6f, 0.0f);
			}
		}
	} else {
		if (transform.GetLocalPosition().y < 1.7f) {
			transform.Translate(0, deltaTime* 5.0f, 0);
			if (transform.GetLocalPosition().y > 1.7f) {
				transform.SetLocalPosition(0, 1.7f, 0.0f);
			}
		}
	}

	GetGameObject().GetComponent<Camera>().SetFOV(fov);

	if (GetGameObject().GetComponent<Transform>().GetLocalRotation().x > 89.0f) {
		GetGameObject().GetComponent<Transform>().SetLocalRotationX(89.0f);
	} else if (GetGameObject().GetComponent<Transform>().GetLocalRotation().x < -89.0f) {
		GetGameObject().GetComponent<Transform>().SetLocalRotationX(-89.0f);
	}
}