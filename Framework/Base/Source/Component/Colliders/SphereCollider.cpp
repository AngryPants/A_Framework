#include "SphereCollider.h"
#include "../../GameObject/GameObject.h"

SphereCollider::SphereCollider(GameObject &gameObject)
	: Collider("SphereCollider", gameObject)
	, radius(0)
{
}

SphereCollider::~SphereCollider()
{
}
