#ifndef PLAYER_MOVEMENT_SCRIPT_H
#define PLAYER_MOVEMENT_SCRIPT_H

#include "../Script/Script.h"

class PlayerMovementScript : public Script {

public:
	//Variable(s)
	float movementSpeed;
	float rotationSpeed;
	bool onGround;

	//Constructor(s) & Destructor
	PlayerMovementScript(GameObject& gameObject);
	virtual ~PlayerMovementScript();

	//Interface Function(s)
	virtual void Update(double deltaTime);
	virtual void OnCollisionStay(const CollisionInfo& _info);

};

#endif