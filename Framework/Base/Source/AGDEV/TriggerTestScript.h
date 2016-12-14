#ifndef TRIGGER_TEST_SCRIPT_H
#define TRIGGER_TEST_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"

class TriggerTestScript : public Script {

public:
	//Constructor(s) & Destructor
	TriggerTestScript(GameObject& gameObject): Script(gameObject) {
	}
	virtual ~TriggerTestScript() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) {}

	virtual void OnTriggerStay(const Collider& _collider) {
		cout << "Fucking Triggered!" << endl;
	}

};

#endif