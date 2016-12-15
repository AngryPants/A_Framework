#include "AABBCollider.h"
#include "../Component/Physics/Collision/ColliderGroup.h"

//Constructor(s) & Destructor
AABBCollider::AABBCollider(GameObjectID _gameObjectID) : Collider("AABB Collider", _gameObjectID) {
	isGround = false;
	size.Set(1, 1, 1);
}

AABBCollider::AABBCollider(const AABBCollider& _rhs) : Collider(_rhs) {
	isGround = _rhs.isGround;
	this->size = _rhs.size;
}

AABBCollider::~AABBCollider() {
}

//Virtual Function(s)
AABBCollider* AABBCollider::Copy() {
	return new AABBCollider(*this);
}