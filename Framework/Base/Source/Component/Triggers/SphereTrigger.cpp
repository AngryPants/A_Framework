#include "SphereTrigger.h"
#include "../../GameObject/GameObject.h"

SphereTrigger::SphereTrigger(GameObject &gameObject)
	: Component("SphereTrigger", gameObject)
	, activeTrigger(true)
	, radius(0)
{
}

SphereTrigger::~SphereTrigger()
{
}

void SphereTrigger::SetRadius(float newRadius)
{
	radius = newRadius;
}

float SphereTrigger::GetRadius()
{
	return radius;
}