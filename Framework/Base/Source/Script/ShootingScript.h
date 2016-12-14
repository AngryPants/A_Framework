#ifndef SHOOTING_SCRIPT_H
#define SHOOTING_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Game/Rifle.h"
#include "../Input/InputManager.h"
#include "../Component/Rendering/MeshHolder.h"

class ShootingScript : public Script {
private:
	//Variable(s)
	double timeLeftBeforeReloaded, timeBeforeNextBullet;
public:

	//Constructor(s) & Destructor
	ShootingScript(GameObject& gameObject) : Script(gameObject)
	{
		timeLeftBeforeReloaded = timeBeforeNextBullet = 0.0f;
	}
	virtual ~ShootingScript() {}

	//Interface Function(s)

	// define the bullet to shoot
	virtual void ShootBullet(){};

	//For Different use cases
	virtual bool ExtraCondition(){ return true;};

	virtual void Update(double deltaTime)
	{
		// if gun is reloading
		if ( GetGameObject().GetComponent<RifleComponent>().isReloading)
		{
			timeLeftBeforeReloaded += deltaTime;
			// if reloaded
			if (timeLeftBeforeReloaded > GetGameObject().GetComponent<RifleComponent>().reloadTime)
			{
				// if there is still magazine
				if (GetGameObject().GetComponent<RifleComponent>().magazineSize > 0)
				{
					// if there is less then the max number of clipsize, just take the whole magazine
					if (GetGameObject().GetComponent<RifleComponent>().clipSize > GetGameObject().GetComponent<RifleComponent>().magazineSize)
					{
						GetGameObject().GetComponent<RifleComponent>().currentClipSize = GetGameObject().GetComponent<RifleComponent>().magazineSize;
						GetGameObject().GetComponent<RifleComponent>().magazineSize = 0;
					}
					// if there is magazine has more then or just enough clips, deduct from it
					else if (GetGameObject().GetComponent<RifleComponent>().magazineSize >= GetGameObject().GetComponent<RifleComponent>().clipSize)
					{
						GetGameObject().GetComponent<RifleComponent>().magazineSize -= GetGameObject().GetComponent<RifleComponent>().clipSize;
						GetGameObject().GetComponent<RifleComponent>().currentClipSize = GetGameObject().GetComponent<RifleComponent>().clipSize;
					}
				}
				GetGameObject().GetComponent<RifleComponent>().isReloading = false;
				timeLeftBeforeReloaded = 0.f;
			}
		}
		// if gun just fired
		else if (GetGameObject().GetComponent<RifleComponent>().fired)
		{
			timeBeforeNextBullet += deltaTime;
			if (timeBeforeNextBullet > 1.f / GetGameObject().GetComponent<RifleComponent>().GetRateOfFire())
			{
				GetGameObject().GetComponent<RifleComponent>().fired = false;
				timeBeforeNextBullet = 0.f;
			}
		}
		// if Rifle is ready to fire
		else if (ExtraCondition() &&
			GetGameObject().GetComponent<RifleComponent>().currentClipSize > 0 &&
			!GetGameObject().GetComponent<RifleComponent>().isReloading &&
			!GetGameObject().GetComponent<RifleComponent>().fired)
		{
			ShootBullet();
			GetGameObject().GetComponent<RifleComponent>().currentClipSize -= 1;
			GetGameObject().GetComponent<RifleComponent>().fired = true;
		}
		// gun just fired
		else if (GetGameObject().GetComponent<RifleComponent>().currentClipSize <= 0 &&
			!GetGameObject().GetComponent<RifleComponent>().isReloading)
		{
			GetGameObject().GetComponent<RifleComponent>().isReloading = true;
		}
	}


};

#endif