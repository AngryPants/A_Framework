#include "AIMovementScript.h"
#include "../GameObject/GameObjectFactory.h"

//constructor and destructor
AIMovementScript::AIMovementScript(GameObject& gameObject) : Script(gameObject)
{
	waypoints.clear();
	current = nullptr;
	moveSpeed = rand() % 10 + 15.f; // 15 - 25 speed
	reachedDestination = false;
}

AIMovementScript::~AIMovementScript()
{
	current = nullptr;
}

void AIMovementScript::OnTriggerStay(const Collider& _collider)
{
	/*if (&_collider == &GetGameObject().GetComponent<ColliderGroup<SphereCollider>>().colliders[2])
	{*/
		GameObject* temp = GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID);
		if (temp->name == "Waypoint" && temp->GetComponent<Transform>().GetPosition() == current->GetComponent<Transform>().GetPosition())
		{
			if (current->GetComponent<WayPointComponent>().next != nullptr)
				current = &current->GetComponent<WayPointComponent>().next->GetGameObject();
			else
				reachedDestination = true;
		}
	//}
}

//Function(s)
void AIMovementScript::Update(double deltaTime)
{
	if (!reachedDestination)
		GetGameObject().GetComponent<Rigidbody>().AddRelativeForce(GetDirectionToNext() * 1000);
	if (GetGameObject().GetComponent<Rigidbody>().velocity.LengthSquared() > moveSpeed* moveSpeed)
	{
		GetGameObject().GetComponent<Rigidbody>().velocity.Normalize() *= moveSpeed;
	}

	GetGameObject().GetComponent<Rigidbody>().velocity.y = 0;
}

void AIMovementScript::CreateWayPoint(const Vector3& waypointPosition, const float& radius)
{
	GameObject& temp = GameObjectFactory::CreateWayPointBase(GetGameObject().GetSpace());
	temp.GetComponent<Transform>().SetLocalPosition(waypointPosition);
	temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].isTrigger = true;
	temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(radius);
	waypoints.push_back(&temp);
}

bool AIMovementScript::SetNewWayPoint(WayPointComponent* current, int position)
{
	if (position - 1 > waypoints.size() || position < 0)
		return false;

	int pos = 0;
	for (vector<GameObject*>::iterator iter = waypoints.begin(); iter != waypoints.end(); iter++)
	{
		GameObject* temp = *iter;

		if (pos == position)
		{
			current->next = &temp->GetComponent<WayPointComponent>();
			current->prev = temp->GetComponent<WayPointComponent>().prev;
			temp->GetComponent<WayPointComponent>().prev->next = current;
			temp->GetComponent<WayPointComponent>().prev = current;
			return true;
		}

		pos++;
	}

	return false;
}

void AIMovementScript::LinkWayPoint()
{
	if (waypoints.size() > 1)
	{
		vector<GameObject*>::iterator start, end;
		start = waypoints.begin();
		end = waypoints.end() - 1;

		(*start)->GetComponent<WayPointComponent>().prev = (&(*(end))->GetComponent<WayPointComponent>());
		(*end)->GetComponent<WayPointComponent>().next = &((*start)->GetComponent<WayPointComponent>());

		for (vector<GameObject*>::iterator iter = waypoints.begin(); iter != waypoints.end(); ++iter)
		{
			if (iter + 1 < waypoints.end())
			{
				(*iter)->GetComponent<WayPointComponent>().next = (&(*(iter + 1))->GetComponent<WayPointComponent>());
				(*(iter + 1))->GetComponent<WayPointComponent>().prev = &((*iter)->GetComponent<WayPointComponent>());
			}
		}
	}

	if (waypoints.size() > 0 && current == nullptr)
		current = *waypoints.begin();

}

Vector3 AIMovementScript::GetDirectionToNext()
{
	if (current != nullptr)
		return ( (current->GetComponent<Transform>().GetPosition() - GetGameObject().GetComponent<Transform>().GetPosition()).Normalized() );

	return Vector3(0, 0, 0);
}