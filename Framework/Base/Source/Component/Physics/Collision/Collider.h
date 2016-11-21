#ifndef COLLIDER_H
#define COLLIDER_H

#include "../../Component.h"
#include "Vector3.h"
#include <vector>

using namespace std;

//Forward Declaration
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