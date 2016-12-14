#include "AIMovementScript.h"
#include "../GameObject/GameObjectFactory.h"

//constructor and destructor
AIMovementScript::AIMovementScript(GameObject& gameObject) : Script(gameObject)
{
	waypoints.clear();
	current = nullptr;
	moveSpeed = 0.05f;
}

AIMovementScript::~AIMovementScript()
{
}

//Function(s)
void AIMovementScript::Update(double deltaTime)
{
	// if gameobject is within target waypoint,shift waypoint to next one
	if (current->GetComponent<WayPointComponent>().trigger)
	{
		current = &current->GetComponent<WayPointComponent>().next->GetGameObject();
	}
	GetGameObject().GetComponent<Transform>().Translate(GetDirectionToNext()* moveSpeed);
}

void AIMovementScript::CreateWayPoint(const Vector3& waypointPosition, const float& radius)
{
	GameObject& temp = GameObjectFactory::CreateWayPointBase(GetGameObject().GetSpace());
	temp.GetComponent<Transform>().SetLocalPosition(waypointPosition);
	temp.GetComponent<SphereCollider>().isTrigger = true;
	temp.GetComponent<SphereCollider>().centre = waypointPosition;
	temp.GetComponent<SphereCollider>().radius = radius;
	waypoints.push_back(&temp);
	UpdateWayPoint();
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

void AIMovementScript::UpdateWayPoint()
{
	if (current == nullptr)
		current = *waypoints.begin();

	if (waypoints.size() > 1)
	{
		vector<GameObject*>::iterator start, end;
		start = waypoints.begin();
		end = waypoints.end() - 1;

		(*start)->GetComponent<WayPointComponent>().next = (&(*(end))->GetComponent<WayPointComponent>());
		(*end)->GetComponent<WayPointComponent>().prev = &((*start)->GetComponent<WayPointComponent>());
/*
		GameObject* temp1 = GameObjectManager::GetInstance().GetGameObjectByID(*start);
		GameObject* temp2 = GameObjectManager::GetInstance().GetGameObjectByID(*end);

		temp1->GetComponent<WayPointComponent>().prev = &temp2->GetComponent<WayPointComponent>();
		temp2->GetComponent<WayPointComponent>().next = &temp1->GetComponent<WayPointComponent>();*/

		for (vector<GameObject*>::iterator iter = waypoints.begin(); iter != waypoints.end(); ++iter)
		{
			if (iter + 1 < waypoints.end())
			{
				(*iter)->GetComponent<WayPointComponent>().next = (&(*(iter + 1))->GetComponent<WayPointComponent>());
				(*(iter + 1))->GetComponent<WayPointComponent>().prev = &((*iter)->GetComponent<WayPointComponent>());

				//GameObject* temp1 = GameObjectManager::GetInstance().GetGameObjectByID((*iter)->GetID());
				//GameObject* temp2 = GameObjectManager::GetInstance().GetGameObjectByID((*(iter + 1))->GetID());

				//temp1->GetComponent<WayPointComponent>().next = &temp2->GetComponent<WayPointComponent>();
				//temp2->GetComponent<WayPointComponent>().prev = &temp1->GetComponent<WayPointComponent>();/**/
			}
		}
	}
	 

}

Vector3 AIMovementScript::GetDirectionToNext()
{
	if (current != nullptr)
		return ( (current->GetComponent<Transform>().GetPosition() - GetGameObject().GetComponent<Transform>().GetPosition()).Normalized() );

	return Vector3(0, 0, 0);
}