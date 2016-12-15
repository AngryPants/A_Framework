#ifndef COLLIDER_H
#define COLLIDER_H

#include "Vector3.h"
#include "../Component.h"
#include <vector>
#include "../../GameObject/GameObject.h"
using std::vector;
//Forward declaration
class GameObject;

//CollisionInfo
class CollisionInfo {

private:
	//Variable(s)
	bool previousState;
	bool currentState;

public:
	//Variable(s)
	vector<GameObject*> gameObjects;
	vector<Vector3> collisionPoint;

	//Constructor(s) & Destructor
	CollisionInfo() {
		gameObjects.clear();
		collisionPoint.clear();
		previousState = false;
		currentState = false;
	}
	virtual ~CollisionInfo() {}

	void Collide(GameObject& gameObject, const Vector3& collisionPoint) {
		currentState = true;
		this->gameObjects.push_back(&gameObject);
		this->collisionPoint.push_back(collisionPoint);
	}

	void Reset() {
		gameObjects.clear();
		collisionPoint.clear();
		previousState = currentState;
		currentState = false;
	}

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

//Colliders are supposed to be Simple and Cheap Contact Checks , MultiPurpose Not only for Collision
class Collider : public Component {

protected:
	//Constructor(s)
	Collider(const string& name, GameObject& gameObject) : Component(name, gameObject) {
		isTrigger = false;
		centre = GetGameObject().GetComponent<Transform>().GetPosition();
	}

public:
	//Variable(s)
	bool isTrigger; // Used for triggering events and ignored by physics engine if set to true 
	Vector3 centre; 

	CollisionInfo info;
	//function(s)
	// Scripts to define what happens when the colliders Enters/Stays/Exits 
	virtual void OnTriggerEnter(){}
	virtual void OnTriggerStay(){}
	virtual void OnTriggerExit(){}

	//Destructor
	virtual ~Collider() {}

};




#endif