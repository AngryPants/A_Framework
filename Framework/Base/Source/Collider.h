#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "Vector3.h"

//Forward Declaration
class GameObject;

//CollisionInfo
class CollisionInfo {

private:
	//Variable(s)
	GameObject* gameObject;
	bool previousState;
	bool currentState;
	Vector3 collisionPoint;

public:
	//Constructor(s) & Destructor
	CollisionInfo() {
		gameObject = nullptr;
		previousState = false;
		currentState = false;
	}
	virtual ~CollisionInfo() {}

	void Collide(bool hasCollision, GameObject* gameObject = nullptr, const Vector3& collisionPoint = Vector3(0, 0, 0)) {
		previousState = currentState;
		if (!hasCollision || gameObject == nullptr) {
			currentState = false;
			this->gameObject = nullptr;
			this->collisionPoint.SetZero();
		} else {
			currentState = true;
			this->gameObject = gameObject;
			this->collisionPoint = collisionPoint;
		}		
	}

	const Vector3& GetCollisionPoint() {
		return this->collisionPoint;
	}

	GameObject* GetGameObject();

	bool EnterCollision() {
		return (currentState == true && previousState == false);
	}
	bool ExitCollision() {
		return (currentState == false && previousState == true);
	}
	bool IsColliding() {
		return currentState;
	}

};

//Collider
class Collider : public Component {

protected:
	//Constructor(s)
	Collider(const string& name, GameObject& gameObject) : Component(name, gameObject) {
		isTrigger = false;
	}

public:
	//Variable(s)
	bool isTrigger;
	Vector3 centre;
	CollisionInfo info;

	//Destructor
	virtual ~Collider() {}
	
};

#endif