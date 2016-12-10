#ifndef PIVOT_SCRIPT_H
#define PIVOT_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"

class PivotScript : public Script {

public:
	//Constructor(s) & Destructor
	PivotScript(GameObject& gameObject): Script(gameObject) {
	}
	virtual ~PivotScript() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) {
		GetGameObject().GetComponent<Transform>().Rotate(0,deltaTime * 60.0f,0);
	}

};

#endif