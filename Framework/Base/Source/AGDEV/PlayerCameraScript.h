#ifndef PLAYER_CAMERA_SCRIPT_H
#define PLAYER_CAMERA_SCRIPT_H

#include "../Script/Script.h"

class PlayerCameraScript : public Script {

public:
	//Variable(s)
	float rotationSpeed;
	float fov;

	//Constructor(s) & Destructor
	PlayerCameraScript(GameObject& gameObject);
	virtual ~PlayerCameraScript();

	//Interface Function(s)
	virtual void Update(double deltaTime);

};

#endif