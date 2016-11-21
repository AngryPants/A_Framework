#ifndef PLAYER_MOVEMENT_SCRIPT_H
#define PLAYER_MOVEMENT_SCRIPT_H

#include "../Script/Script.h"

class PlayerMovementScript : public Script {

public:
	//Variable(s)
	float movementSpeed;
	float rotationSpeed;

	//Constructor(s) & Destructor
	PlayerMovementScript(GameObject& gameObject);
	virtual ~PlayerMovementScript();

	//Interface Function(s)
	virtual void Update(double deltaTime);

};

#endif