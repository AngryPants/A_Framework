#include "Rigidbody.h"
#include "../ComponentManager.h"
#include "../../Systems/Physics/PhysicsSystem.h"

//Constructor(s) & Destructor
Rigidbody::Rigidbody(GameObject& _gameObject, PassKey<ComponentManager> _key)
	: Component("rigidBody", _gameObject, _key)
	, mass(1.0f)
	, elasticity(0.05f)
	, velocity(0,0,0)
	, deltaTime(Math::EPSILON)
	, gravity(DEFAULT_GRAVITY)
	, useGravity(true)
	, sleeping(false)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::SetVelocity(Vector3 _velocity)
{
	this->velocity = _velocity;
}

void Rigidbody::SetVelocity(float _x, float _y, float _z)
{
	this->velocity.Set(_x, _y, _z);
}

Vector3 Rigidbody::GetVelocity() const
{
	return velocity;
}

void Rigidbody::AddRelativeForce(float _x, float _y, float _z, FORCE_MODE _mode) {
	AddRelativeForce(Vector3(_x, _y, _z), _mode);
}

void Rigidbody::AddRelativeForce(Vector3 _force, FORCE_MODE _mode) {

	switch (_mode) {
		case FORCE_MODE::FM_FORCE: {
			this->velocity += (_force * (1.0f / this->mass)) * deltaTime;
		}
		break;
		case FORCE_MODE::FM_ACCELERATION: {
			this->velocity += _force * deltaTime;
		}
		break;
		case FORCE_MODE::FM_IMPULSE: {
			this->velocity += _force * (1.0f / this->mass);
		}
		break;
		case FORCE_MODE::FM_VELOCITY: {
			this->velocity += _force;
		}
		break;
		default: {
			//Do nothing
		}			
	}

}

void Rigidbody::SetMass(float _mass)
{
	this->mass = Math::Max(0.001f, _mass);
}

float Rigidbody::GetMass()
{
	return mass;
}

void Rigidbody::SetDeltaTime(double _deltaTime, PassKey<PhysicsSystem> _key) {
	this->deltaTime = _deltaTime;
}