#ifndef HEALTH_SCRIPT_H
#define HEALTH_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h" 
#include "../Component/Game/Health.h"

class HealthScript : public Script {
public:
	//Constructor(s) & Destructor
	HealthScript(GameObject& gameObject) : Script(gameObject)
	{
	}
	virtual ~HealthScript() {}

	//Interface Function(s)
	virtual void Update(double deltaTime)
	{
		if (GetGameObject().GetComponent<HealthComponent>().GetCurrentHealth() <= 0)
			ZeroHp();
	}

	virtual void ZeroHp() { GetGameObject().Destroy(); }
};

#endif