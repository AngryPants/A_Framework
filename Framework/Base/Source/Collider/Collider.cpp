#include "Collider.h"
#include "../Component/Physics/Collision/ColliderGroup.h"

//Constructor(s) & Destructor
Collider::Collider(const string& _name, GameObjectID _gameObjectID) : name(_name), gameObjectID(_gameObjectID) {
	this->isTrigger = false;
	this->isActive = true;
	this->centre.SetZero();
}

Collider::Collider(const Collider& _rhs) : name(_rhs.name), gameObjectID(_rhs.gameObjectID) {
	this->isTrigger = _rhs.isTrigger;
	this->isActive = _rhs.isActive;
	this->centre = _rhs.centre;
}

Collider::~Collider() {
}