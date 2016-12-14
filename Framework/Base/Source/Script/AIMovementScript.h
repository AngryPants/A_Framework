#ifndef AI_MOVEMENT_SCRIPT_H
#define AI_MOVEMENT_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Game/Rifle.h"
#include "../Input/InputManager.h"
#include "../Component/Game/Waypoint.h"

class AIMovementScript : public Script
{
public:
	//Variables
	vector<GameObjectID> waypoints;
	GameObjectID current;
	float moveSpeed;

	//constructor and destructor
	AIMovementScript(GameObject& gameObject);
	virtual ~AIMovementScript();

	//Function(s)
	virtual void Update(double deltaTime);
	void CreateWayPoint(const Vector3& waypointPosition,const float& radius);
	bool SetWayPoint(WayPointComponent* current, int position);
	void UpdateWayPoint();
	Vector3 GetDirectionToNext();
};


#endif