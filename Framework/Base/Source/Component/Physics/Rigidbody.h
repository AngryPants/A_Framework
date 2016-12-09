#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Component.h"
#include "Vector3.h"

class Rigidbody : public Component{

private:
	//Variable(s)
	float mass;
	
public:
	Vector3 velocity;
	Vector3 gravity;
	
	bool useGravity;
	bool isKinematic;
	
	//Constructor(s) & Destructor
	Rigidbody(GameObject& gameObject);
	virtual ~Rigidbody();
};

#endif