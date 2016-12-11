#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Component.h"
#include "Vector3.h"

//If RigidBody component is attached, use apply forces to move it unless isKinematics is true,then do your own version of movement
//Collision Should not happen if RigidBody does not have a box Collider
class Rigidbody : public Component{

private:
	//Variable(s)
	float mass; // the mass of the object
	bool sleeping;// when the object doesnt move,set to sleep for more efficient Collision Checking
	//unsigned int drag (from 0-1 to calculate air resistance)
	//unsigned AngularDrag( from 0-1 to calculate how much air resistance affects the object when rotating from torque ,0 means no resistance)
	Vector3 velocity;  // shouldn't be access or changed directly by anyone other then the physics engine
	enum ForceMode
	{
		Force, //Add a continuous force to the rigidbody, using its mass.
		Acceleration, //Add a continuous acceleration to the rigidbody, ignoring its mass.
		Impulse, //Add an instant force impulse to the rigidbody, using its mass.
		VelocityChange, //Add an instant velocity change to the rigidbody, ignoring its mass.
	};
	ForceMode mode;
public:
	//Constructor(s) & Destructor
	Rigidbody(GameObject& gameObject);
	virtual ~Rigidbody();

	//public variable(s)
	Vector3 gravity;
	
	bool useGravity; // Apply Gravity
	bool isKinematic; // Physics Engine will ignore this as well
	
	// public function(s)
	Vector3 GetVelocity() const;
	void AddForce(float x, float y, float z, unsigned int forceMode = Force);
	void AddForce(Vector3 force, unsigned int forceMode = Force);
	//void AddTorque(); // Next time ba, for rotating the object
	
	void Sleep();
	void WakeUp();
	bool IsSleeping();

	void SetMass(float mass);
	float GetMass();
	
	virtual void OnCollisionEnter(){}
	virtual void OnCollisionStay(){}
	virtual void OnCollisionExit(){}
};

#endif