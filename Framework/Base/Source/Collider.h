#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "Vector3.h"

class Collider : public Component {

private:
	bool previousState;
	bool currentState;

protected:
	//Constructor(s)
	Collider(const string& name, GameObject& gameObject) : Component(name, gameObject) {
		isTrigger = false;
		previousState = false;
		currentState = false;
	}

public:
	//Variable(s)
	bool isTrigger;
	Vector3 centre;

	//Destructor
	virtual ~Collider() {}

	//Interface Function(s)
	void Collide(const bool hasCollision) {
		previousState = currentState;
		currentState = hasCollision;
	}

};

#endif