#ifndef SCALE_SCRIPT_H
#define SCALE_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"

class ScaleScript : public Script {

public:
	//Variable(s)
	double timePassed;
	float min, max;

	//Constructor(s) & Destructor
	ScaleScript(GameObject& gameObject): Script(gameObject) {
		timePassed = 0.0f;
		min = 0.1f;
		max = 1.0f;
	}
	virtual ~ScaleScript() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) {
		timePassed += deltaTime;
		float scale = Math::Clamp(static_cast<float>(sin(timePassed)) * 0.05f, min, max);
		GetGameObject().GetComponent<Transform>().SetLocalScale(scale,scale,scale);
	}

};

#endif