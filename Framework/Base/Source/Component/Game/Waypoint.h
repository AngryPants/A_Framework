#ifndef WAYPOINT_COMPONENT_H
#define WAYPOINT_COMPONENT_H

#include "../Component.h"
#include "Vector3.h"

class WayPointComponent: public Component
{
public:
	//constructor & destructor
	WayPointComponent(GameObject& gameObject, PassKey<ComponentManager> _key)
		: Component("Waypoint Component", gameObject, _key)
	{
		prev = next = nullptr;
		trigger = false;
	};
	virtual ~WayPointComponent() {}
	//variables
	bool trigger;
	WayPointComponent *prev, *next;
};

#endif