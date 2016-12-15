#ifndef SHOOTING_SCRIPT_H
#define SHOOTING_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Game/Rifle.h"
#include "../Input/InputManager.h"
#include "../Component/Rendering/MeshHolder.h"

class ShootingScript : public Script {
private:
	//Variable(s)
	double timeLeftBeforeReloaded, timeBeforeNextBullet;
public:

	//Constructor(s) & Destructor
	ShootingScript(GameObject& gameObject);
	virtual ~ShootingScript() {}

	//Interface Function(s)

	virtual void OnTriggerStay(const Collider& _collider){}
	// define the bullet to shoot
	virtual void ShootBullet(){}

	//Manual reload
	virtual bool ManualReload(){ return false; }

	//For Different use cases
	virtual bool ExtraCondition(){ return false; }

	//default firing
	virtual void ShootingLogic(double deltaTime);

	virtual void Update(double deltaTime);

	Vector3 LookAt(Vector3 target, Vector3 up);
};

#endif