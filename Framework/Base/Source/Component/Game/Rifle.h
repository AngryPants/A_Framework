#ifndef RIFLE_COMPONENT_H
#define RIFLE_COMPONENT_H

#include "../Rendering/LODMeshHolder.h"
#include "../../GameObject/GameObject.h"
#include "Vector3.h"

class RifleComponent : public Component
{
private:
	float rateOfFire;// time between firing each round
public:
	//variables 
	float reloadTime;// time between reloading a clip
	unsigned int currentClipSize;// current Number of bullets
	unsigned int clipSize; // number of bullets per clip
	unsigned int magazineSize; // number of totalBullets
	bool isReloading;
	bool fired;
	bool isHeld;// whether gun is being hold
	//Constructors
	RifleComponent(GameObject& gameObject, PassKey<ComponentManager> _key)
		: Component("Rifle Component", gameObject, _key)
		, rateOfFire(0.f)
		, reloadTime(0.f)
		, currentClipSize(0)
		, clipSize(0)
		, magazineSize(0)
		, isReloading(false)
		, fired(false)
		, isHeld(false)
	{		
	}
	virtual ~RifleComponent(){}

	//functions
	bool SetRateOfFire(const float& rateOfFire)
	{
		if (rateOfFire > 0.f)
		{
			this->rateOfFire = rateOfFire;
			return true;
		}
		return false;
	}

	float GetRateOfFire()
	{
		return rateOfFire;
	}
};
#endif