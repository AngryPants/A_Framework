#include "PlayerCameraScript.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Rendering/Camera.h"

//Constructor(s) & Destructor
PlayerCameraScript::PlayerCameraScript(GameObject& gameObject) : Script(gameObject) {
	rotationSpeed = 360.0f;
	fov = GetGameObject().GetComponent<Camera>().GetFOV();
}

PlayerCameraScript::~PlayerCameraScript() {
}

//Interface Function(s)
void PlayerCameraScript::Update(double deltaTime) {	
	float rotation = InputManager::GetInstance().GetInputInfo().axis[IAXIS_LOOK_VERTICAL];
	if (rotation != 0.0f) {
		GetGameObject().GetComponent<Transform>().Rotate(rotationSpeed * deltaTime * rotation, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_UP]) {
		GetGameObject().GetComponent<Transform>().Rotate(-deltaTime * rotationSpeed, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_DOWN]) {
		GetGameObject().GetComponent<Transform>().Rotate(deltaTime * rotationSpeed, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_ZOOM]) {
		fov = Math::Max(20.0f, fov - static_cast<float>(deltaTime) * 360.0f);
	} else {
		fov = Math::Min(60.0f, fov + static_cast<float>(deltaTime) * 360.0f);
	}
	GetGameObject().GetComponent<Camera>().SetFOV(fov);

	if (GetGameObject().GetComponent<Transform>().GetLocalRotation().x > 90.0f) {
		GetGameObject().GetComponent<Transform>().SetLocalRotationX(90.0f);
	} else if (GetGameObject().GetComponent<Transform>().GetLocalRotation().x < -90.0f) {
		GetGameObject().GetComponent<Transform>().SetLocalRotationX(-90.0f);
	}
}