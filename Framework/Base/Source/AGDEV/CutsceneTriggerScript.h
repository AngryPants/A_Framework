#ifndef CUTSCENE_TRIGGER_SCRIPT_H
#define CUTSCENE_TRIGGER_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h" 

class CutsceneTriggerScript : public Script {
public:
	//Variable(s)
	double timer;
	bool play;

	GameObject* camPivot;
	GameObject* camera;

	//Constructor(s) & Destructor
	CutsceneTriggerScript(GameObject& gameObject);
	virtual ~CutsceneTriggerScript();

	//Interface Function(s)
	virtual void Update(double deltaTime);

	virtual void OnTriggerStay(const Collider& _collider);
};

#endif