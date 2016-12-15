#include "PlayerMovementScript.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Physics/Rigidbody.h"

//Constructor(s) & Destructor
PlayerMovementScript::PlayerMovementScript(GameObject& gameObject) : Script(gameObject) {
	movementSpeed = 5.0f;
	rotationSpeed = 360.0f;
}

PlayerMovementScript::~PlayerMovementScript() {
}

//Interface Function(s)
void PlayerMovementScript::Update(double deltaTime) {
	Rigidbody& rigidbody = GetGameObject().AddComponent<Rigidbody>();
	rigidbody.useGravity = false;
	bool keyPressed = false;
	Transform& transform = GetGameObject().GetComponent<Transform>();	
	transform.Rotate(0, deltaTime * rotationSpeed * InputManager::GetInstance().GetInputInfo().axis[IAXIS_LOOK_HORIZONTAL], 0);
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_LEFT]) {
		transform.Translate(movementSpeed * deltaTime * transform.GetLocalLeft());
		keyPressed = true;
	}
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_RIGHT]) {
		transform.Translate(movementSpeed * deltaTime * -transform.GetLocalLeft());
		keyPressed = true;
	}
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_FORWARD]) {
		rigidbody.AddRelativeForce(transform.GetForward() * 500);
		//transform.Translate(movementSpeed * deltaTime * transform.GetLocalForward());
		keyPressed = true;
	}
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_BACKWARD]) {
		transform.Translate(movementSpeed * deltaTime * -transform.GetLocalForward());
		keyPressed = true;
	}
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_LEFT]) {
		transform.Rotate(0,deltaTime * rotationSpeed, 0);
		keyPressed = true;
	}
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_RIGHT]) {
		transform.Rotate(0,-deltaTime * rotationSpeed, 0);
		keyPressed = true;
	}

	if (rigidbody.velocity.LengthSquared() > movementSpeed*movementSpeed)
	{
		rigidbody.velocity.Normalize() *= movementSpeed;
	}

	if (!keyPressed)
	{
		rigidbody.velocity.SetZero();
	}
}