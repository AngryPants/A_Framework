#include "ShootingScript.h"

//Constructor(s) & Destructor
ShootingScript::ShootingScript(GameObject& gameObject) : Script(gameObject)
{
	//if (!GetGameObject().HasComponent<RifleComponent>())
	//GetGameObject().RemoveScript(this);

	timeLeftBeforeReloaded = timeBeforeNextBullet = 0.0f;
}

//Interface Function(s)
void ShootingScript::Update(double deltaTime)
{
	if (GetGameObject().HasComponent<RifleComponent>() &&
		GetGameObject().GetComponent<RifleComponent>().isHeld)
		ShootingLogic(deltaTime);
}

void ShootingScript::ShootingLogic(double deltaTime)
{
	// if gun is reloading
	if (GetGameObject().GetComponent<RifleComponent>().isReloading)
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