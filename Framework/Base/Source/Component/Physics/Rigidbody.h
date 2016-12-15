#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Component.h"
#include "Vector3.h"

//Forward Declaration
class PhysicsSystem;

const static Vector3 DEFAULT_GRAVITY(0.0f, -9.81f, 0.0f);

enum class FORCE_MODE {
	FM_FORCE,
	FM_ACCELERATION,
	FM_IMPULSE,
	FM_VELOCITY,
};

//If RigidBody component is attached, use apply forces to move it unless isKinematics is true,then do your own version of movement
//Collision Should not happen if RigidBody does not have a box Collider
class Rigidbody : public Component{

private:
	//Variable(s)
	float mass; // the mass of the object
	bool sleeping;// when the object doesnt move,set to sleep for more efficient Collision Checking
	double deltaTime; //The deltaTime for this frame.
	
public:
	//Constructor(s) & Destructor
	Rigidbody(GameObject& _gameObject, PassKey<ComponentManager> _key);
	virtual ~Rigidbody();

	//public variable(s)
	Vector3 gravity;
	//The velocity of this Rigidbody.
	Vector3 velocity;  //shouldn't be access or changed directly by anyone other then the physics engine

	bool useGravity; //Apply Gravity
	bool isKinematic; //Physics Engine will ignore this as well
	float elasticity;

	// public function(s)
	Vector3 GetVelocity() const;
	void SetVelocity(Vector3 _velocity);
	void SetVelocity(float _x, float _y, float _z);

	void AddRelativeForce(float _x, float _y, float _z, FORCE_MODE _mode = FORCE_MODE::FM_FORCE);
	void AddRelativeForce(Vector3 _force, FORCE_MODE _mode = FORCE_MODE::FM_FORCE);

	void SetMass(float _mass);
	float GetMass();
	
	//Set the deltaTime for this frame.
	void SetDeltaTime(double _deltaTime, PassKey<PhysicsSystem> _key);

	virtual void OnCollisionEnter(){}
	virtual void OnCollisionStay(){}
	virtual void OnCollisionExit(){}
};

#endif