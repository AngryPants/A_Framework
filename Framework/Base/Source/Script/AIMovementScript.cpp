#include "AIMovementScript.h"
#include "../GameObject/GameObjectFactory.h"

//constructor and destructor
AIMovementScript::AIMovementScript(GameObject& gameObject) : Script(gameObject)
{
	waypoints.clear();
	current = 0;
	moveSpeed = 5.f;
}

AIMovementScript::~AIMovementScript()
{

}

//Function(s)
void AIMovementScript::Update(double deltaTime)
{
	// if gameobject is within target waypoint,shift waypoint to next one
	//if (GetGameObject().GetComponent<Transform>().GetPosition < GameObjectManager::GetInstance().GetGameObjectByID(current)->get)
	//{
	//	GameObjectManager::GetInstance().GetGameObjectByID(current)->GetComponent<WayPointComponent>() = GameObjectManager::GetInstance().GetGameObjectByID(current)->GetComponent<WayPointComponent>().next;
	//}
	GetGameObject().GetComponent<Transform>().Translate(GetDirectionToNext()* moveSpeed);
}

void AIMovementScript::CreateWayPoint(const Vector3& waypointPosition, const float& radius)
{
	GameObject& temp = GameObjectFactory::CreateWayPoint(GetGameObject().GetSpace(), "waypoint", waypointPosition, radius);
	waypoints.push_back(temp.GetID());
	UpdateWayPoint();
}

bool AIMovementScript::SetWayPoint(WayPointComponent* current, int position)
{
	if (position - 1 > waypoints.size() || position < 0)
		return false;

	int pos = 0;
	for (vector<GameObjectID>::iterator iter = waypoints.begin(); iter != waypoints.end(); iter++)
	{
		GameObject* temp = GameObjectManager::GetInstance().GetGameObjectByID(*iter);

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
	if (waypoints.size() > 1)
	{
		vector<GameObjectID>::iterator start, end;
		start = waypoints.begin();
		end = waypoints.end();

		GameObject* temp1 = GameObjectManager::GetInstance().GetGameObjectByID(*start);
		GameObject* temp2 = GameObjectManager::GetInstance().GetGameObjectByID(*end);

		temp1->GetComponent<WayPointComponent>().prev = &temp2->GetComponent<WayPointComponent>();
		temp2->GetComponent<WayPointComponent>().next = &temp1->GetComponent<WayPointComponent>();
	}

	for (vector<GameObjectID>::iterator iter = waypoints.begin(); iter != waypoints.end(); iter++)
	{
		GameObject* temp1 = GameObjectManager::GetInstance().GetGameObjectByID(*iter);
		GameObject* temp2 = GameObjectManager::GetInstance().GetGameObjectByID(*(iter + 1));

		if (iter + 1 <= waypoints.end())
		{
			temp1->GetComponent<WayPointComponent>().next = &temp2->GetComponent<WayPointComponent>();
			temp2->GetComponent<WayPointComponent>().prev = &temp1->GetComponent<WayPointComponent>();
		}
	}
}

Vector3 AIMovementScript::GetDirectionToNext()
{
	GameObject * temp = GameObjectManager::GetInstance().GetGameObjectByID(current); 
	if (temp != nullptr)
		return ( (temp->GetComponent<Transform>().GetPosition() - GetGameObject().GetComponent<Transform>().GetPosition()).Normalized() );

	return Vector3(0, 0, 0);
}