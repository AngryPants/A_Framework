#ifndef SCALE_SCRIPT_H
#define SCALE_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"

class ScaleScript : public Script {

public:
	//Variable(s)
	double timePassed;

	//Constructor(s) & Destructor
	ScaleScript(GameObject& gameObject): Script(gameObject) {
		timePassed = 0.0f;
	}
	virtual ~ScaleScript() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) {
		timePassed += deltaTime;
		float offset = sin(timePassed);
		float scale = Math::Max(offset + 1.0f,0.1f);
		GetGameObject().GetComponent<Transform>().SetLocalScale(scale,scale,scale);
	}

};

#endif