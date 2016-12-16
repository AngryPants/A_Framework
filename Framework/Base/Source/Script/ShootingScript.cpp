#include "ShootingScript.h"

//Constructor(s) & Destructor
ShootingScript::ShootingScript(GameObject& gameObject) : Script(gameObject)
{
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
	// if gun is reloading or player Manual Reloads
	if (GetGameObject().GetComponent<RifleComponent>().isReloading == true|| ManualReload() )
	{
		timeLeftBeforeReloaded += deltaTime;
		// if reloaded
		if (timeLeftBeforeReloaded >= GetGameObject().GetComponent<RifleComponent>().reloadTime)
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
			else
			{
				GetGameObject().GetComponent<RifleComponent>().currentClipSize = 0; 
			}
			GetGameObject().GetComponent<RifleComponent>().isReloading = false;
			GetGameObject().GetComponent<RifleComponent>().fired = false;
			timeLeftBeforeReloaded = 0.f;
			timeBeforeNextBullet = 0.f;
		}
	}
	// if gun just fired
	else if (GetGameObject().GetComponent<RifleComponent>().fired == true)
	{
		timeBeforeNextBullet += deltaTime;
		if (timeBeforeNextBullet > 1.f / GetGameObject().GetComponent<RifleComponent>().GetRateOfFire())
		{
			GetGameObject().GetComponent<RifleComponent>().fired = false;
			timeBeforeNextBullet = 0.f;
		}
	}
	// gun has to reload
	else if (GetGameObject().GetComponent<RifleComponent>().currentClipSize <= 0 &&
		GetGameObject().GetComponent<RifleComponent>().isReloading == false)
	{
		GetGameObject().GetComponent<RifleComponent>().isReloading = true;
	}
	// if Rifle is ready to fire
	else if (ExtraCondition()/* &&*/
		/*GetGameObject().GetComponent<RifleComponent>().currentClipSize > 0 &&
		GetGameObject().GetComponent<RifleComponent>().isReloading == false &&
		GetGameObject().GetComponent<RifleComponent>().fired == false*/)
	{ 
		ShootBullet();
		GetGameObject().GetComponent<RifleComponent>().currentClipSize -= 1;
		GetGameObject().GetComponent<RifleComponent>().fired = true;
	}
}

Vector3 ShootingScript::LookAt(Vector3 _target, Vector3 _up) 
{
	//Ensure that the parameters are valid.
	Vector3 desiredView = _target - GetGameObject().GetComponent<Transform>().GetPosition();
	Vector3 desiredLeft = _up.Cross(desiredView);
	_up = desiredLeft.Cross(desiredLeft);
	try {
		desiredView.Normalize();
		desiredLeft.Normalize();
		_up.Normalize();
	}
	catch (DivideByZero) {
		//cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		//return Vector3(0, 0, 0);
	}
	if (desiredView.IsZero() || desiredLeft.IsZero() || _up.IsZero()) {
		//cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		//return Vector3(0, 0, 0);
	}

	//Let's move on to calculations. We want to find our what our global rotation would be if we were looking at the target.
	//First we need to rotate on the Y-Axis until our View aligns with the YZ-Plane.
	//To do that we need our projection on the XZ-Plane, which is the View vector without it's Y-Value.
	float y = Math::RadianToDegree(atan2(desiredView.x, desiredView.z));
	//Then, we need to find our how much we rotated on the X-Axis.
	float x = Math::RadianToDegree(-asin(desiredView.y));
	//To find our Z-Rotation, we first need to know what our left vector would be if we had a Z-Rotation of 0,
	//The expected left vector if Z-Rotation is 0:
	Vector3 expectedLeft(desiredView.z, 0, -desiredView.x);
	//The expected up vector id Z-Rotation is 0:
	Vector3 expectedUp = desiredView.Cross(expectedLeft);
	float z = 0.0f;
	try {
		z = Math::RadianToDegree(atan2(desiredView.Dot(expectedUp.Normalize()), desiredView.Dot(expectedLeft.Normalize())));
	}
	catch (DivideByZero) {
		//cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		//return Vector3(0,0,0);
	}

	//Now we know that we would need to be looking at Vector3(x, y, z) for our global rotation.
	//However, since the whole transform relies on our localRotation, we need to convert it.
	//Only works if there is no Parent 

	return Vector3(x, y, z);
}