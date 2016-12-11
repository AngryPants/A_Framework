#include "BoxCollider.h"
#include "../../GameObject/GameObject.h"

BoxCollider::BoxCollider(GameObject &gameObject)
	: Collider("BoxCollider", gameObject)
	, boxMin(0,0,0)
	, boxMax(0,0,0)
{
}

BoxCollider::~BoxCollider()
{
}