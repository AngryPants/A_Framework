#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Collider/CollisionInfo.h"
#include "../Collider/Collider.h"

class GameObject;

class Script {

private:
	//Variable(s)
	GameObject* gameObject; //The GameObject that owns this Script.

public:
	//Constructor(s) & Destructor
	Script(GameObject& gameObject);
	virtual ~Script();

	//Interface Function(s)
	GameObject& GetGameObject();
	virtual void Update(double deltaTime) = 0;	

	//Callbacks
	virtual void OnCollisionEnter(const CollisionInfo& _info);
	virtual void OnCollisionStay(const CollisionInfo& _info);
	virtual void OnCollisionExit(const CollisionInfo& _info);

	//virtual void OnCollisionEnter(const Collider& _info);
	//virtual void OnCollisionStay(const Collider& _info);
	//virtual void OnCollisionExit(const Collider& _info);

	virtual void OnTriggerEnter(const Collider& _collider);
	virtual void OnTriggerStay(const Collider& _collider);
	virtual void OnTriggerExit(const Collider& _collider);

};

#endif