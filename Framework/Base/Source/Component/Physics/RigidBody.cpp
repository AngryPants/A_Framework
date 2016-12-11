#include "Rigidbody.h"

Rigidbody::Rigidbody(GameObject& gameObject)
	: Component("rigidBody", gameObject)
	, mass(0.f)
	, velocity(0,0,0)
	, gravity(0,0,0)
	, useGravity(false)
	, isKinematic(false)
	, sleeping(true)
	, mode(Force)
{
}

Rigidbody::~Rigidbody()
{

}

Vector3 Rigidbody::GetVelocity() const
{
	return velocity;
}

void Rigidbody::AddForce(float x, float y, float z, unsigned int forceMode)
{
	Vector3 newForce(x, y, z);
	velocity += newForce;
	if (forceMode >= Force && forceMode <= VelocityChange)
		mode = ForceMode(forceMode);
	//should make use of mode to determine the way it works, does nothing for now
}

void Rigidbody::AddForce(Vector3 force, unsigned int forceMode)
{
	velocity += force;
	if (forceMode >= Force && forceMode <= VelocityChange)
		mode = ForceMode(forceMode);
	//should make use of mode to determine the way it works, does nothing for now
}

void Rigidbody::SetMass(float mass)
{
	this->mass = mass;
}

float Rigidbody::GetMass()
{
	return mass;
}

void Rigidbody::Sleep()
{
	sleeping = true;
}

void Rigidbody::WakeUp()
{
	sleeping = false;
}

bool Rigidbody::IsSleeping()
{
	return sleeping;
}