#include "PlayerMovementScript.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Physics/Rigidbody.h"

//Constructor(s) & Destructor
PlayerMovementScript::PlayerMovementScript(GameObject& gameObject) : Script(gameObject) {
	movementSpeed = 10.0f;
	rotationSpeed = 80.0f;
	onGround = true;
}

PlayerMovementScript::~PlayerMovementScript() {
}

//Interface Function(s)
void PlayerMovementScript::Update(double deltaTime) {
	bool keyPressed = false;
	Rigidbody& rigidbody = GetGameObject().AddComponent<Rigidbody>();	
	Transform& transform = GetGameObject().GetComponent<Transform>();	
	transform.Rotate(0, rotationSpeed * InputManager::GetInstance().GetInputInfo().axis[IAXIS_LOOK_HORIZONTAL], 0);
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_LEFT]) {
		rigidbody.AddRelativeForce(transform.GetLeft() * 50);
		//transform.Translate(movementSpeed * deltaTime * transform.GetLocalLeft());
		keyPressed = true;
	}
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_RIGHT]) {
		rigidbody.AddRelativeForce(-transform.GetLeft() * 50);
		//transform.Translate(movementSpeed * deltaTime * -transform.GetLocalLeft());
		keyPressed = true;
	}
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_FORWARD]) {
		rigidbody.AddRelativeForce(transform.GetForward() * 50);
		//transform.Translate(movementSpeed * deltaTime * transform.GetLocalForward());
		keyPressed = true;
	}
	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_BACKWARD]) {
		rigidbody.AddRelativeForce(-transform.GetForward() * 50);
		//transform.Translate(movementSpeed * deltaTime * -transform.GetLocalForward());
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

	if(InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MOVE_JUMP] && onGround) {
		GetGameObject().GetComponent<Rigidbody>().AddRelativeForce(0, 12, 0, FORCE_MODE::FM_IMPULSE);
		onGround = false;
		keyPressed = true;
	}

	Vector3 horizontalVelocity(rigidbody.velocity.x, 0, rigidbody.velocity.z);
	if (horizontalVelocity.LengthSquared() > movementSpeed*movementSpeed)
	{
		horizontalVelocity.Normalize() *= movementSpeed;
		rigidbody.velocity.x = horizontalVelocity.x;
		rigidbody.velocity.z = horizontalVelocity.z;
	}

	if (!keyPressed)
	{
		rigidbody.velocity.x *= 0.8f * (1.0 - deltaTime);
		rigidbody.velocity.z *= 0.8f * (1.0 - deltaTime);
	}

	if (onGround) {
		rigidbody.useGravity = false;
	} else {
		rigidbody.useGravity = true;
	}

	onGround = false;
}

void PlayerMovementScript::OnCollisionStay(const CollisionInfo& _info) {
	GameObject* ground = GameObjectManager::GetInstance().GetGameObjectByID(_info.gameObject);
	//cout << ground->name << endl;
	if (ground->name == "Ground" || ground->name == "Platform") {
		onGround = true;
	}
}