#include "AABBTrigger.h"
#include "../../GameObject/GameObject.h"

AABBTrigger::AABBTrigger(GameObject &gameObject)
	: Component("AABBTrigger", gameObject)
	, activeTrigger(true)
	, AABBMin(0,0,0)
	, AABBMax(0,0,0)
{
}

AABBTrigger::~AABBTrigger()
{
}

void AABBTrigger::SetAABB(Vector3 AABBMin, Vector3 AABBMax)
{
	this->AABBMin = AABBMin;
	this->AABBMax = AABBMax;
}

// Assuming Size is the Size of the Object from the center
void AABBTrigger::SetAABB(Vector3 size)
{
	AABBMin = GetGameObject().GetComponent<Transform>().GetPosition() - size;
	AABBMax = GetGameObject().GetComponent<Transform>().GetPosition() + size;
}

//Assuming Size is Equal for all sides
void AABBTrigger::SetAABB(float x, float y, float z)
{
	Vector3 size(x, y, z);
	AABBMin = GetGameObject().GetComponent<Transform>().GetPosition() - size;
	AABBMax = GetGameObject().GetComponent<Transform>().GetPosition() + size;
}

void AABBTrigger::GetAABB(Vector3& AABBMin, Vector3& AABBMax)
{
	AABBMin = GetGameObject().GetComponent<Transform>().GetPosition() + GetMinAABB();
	AABBMax = GetGameObject().GetComponent<Transform>().GetPosition() + GetMaxAABB();
}

Vector3 AABBTrigger::GetMinAABB()
{
	return AABBMin;
}

Vector3 AABBTrigger::GetMaxAABB()
{
	return AABBMax;
}
