#ifndef ROTATE_SCRIPT_H
#define ROTATE_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"

class RotateScript : public Script {

public:
	//Variable(s)
	
	//Constructor(s) & Destructor
	RotateScript(GameObject& gameObject) : Script(gameObject) {}
	virtual ~RotateScript() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) {
		GetGameObject().GetComponent<Transform>().Rotate(50 * deltaTime,50 * deltaTime, 0);		
	}

};

#endif