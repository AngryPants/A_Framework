#ifndef TRANSLATE_SCRIPT_H
#define TRANSLATE_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"

class TranslateScript : public Script {

public:
	//Variable(s)
	double timePassed;

	//Constructor(s) & Destructor
	TranslateScript(GameObject& gameObject): Script(gameObject) {
		timePassed = 0.0f;
	}
	virtual ~TranslateScript() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) {
		timePassed += deltaTime;
		float offset = sin(timePassed);
		GetGameObject().GetComponent<Transform>().SetLocalPosition(offset * 5.0f, 0, 0);
	}

};

#endif