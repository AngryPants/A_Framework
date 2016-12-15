#ifndef COLLIDER_H
#define COLLIDER_H

#include "PassKey.h"
#include "Vector3.h"
#include "../Others/IDGenerator.h"
#include <vector>
#include <set>
#include <string>

using std::vector;
using std::set;
using std::string;

//Colliders are supposed to be Simple and Cheap Contact Checks, MultiPurpose Not only for Collision
class Collider {

protected:
	//Constructor(s)
	Collider(const string& _name, GameObjectID _gameObjectID);
	Collider(const Collider& _rhs);

public:
	//Variable(s)
	const GameObjectID gameObjectID;
	const string name;
	bool isTrigger; // Used for triggering events and ignored by physics engine if set to true 
	bool isActive;
	Vector3 centre;
	set<string> tags;

	//Destructor
	virtual ~Collider();

	//Virtual Function(s)
	virtual Collider* Copy() = 0;

};

#endif