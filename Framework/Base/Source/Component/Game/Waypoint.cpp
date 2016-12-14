#include "Waypoint.h"
#include "../ComponentManager.h"

//constructor & destructor
WayPointComponent::WayPointComponent(GameObject& gameObject, PassKey<ComponentManager> _key)
	: Component("Waypoint Component", gameObject, _key)
{
	prev = next = nullptr;
	trigger = false;
}