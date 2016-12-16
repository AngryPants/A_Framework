#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H


#include "../Component.h"

class PlayerDataComponent : public Component
{
public:
	bool isHoldingGun;

	PlayerDataComponent(GameObject& gameObject, PassKey<ComponentManager> _key) : Component("Player's Data", gameObject, _key)
	{
		isHoldingGun = false;
	}
	virtual ~PlayerDataComponent(){}

};





#endif