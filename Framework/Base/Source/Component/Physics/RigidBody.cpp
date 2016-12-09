#include "Rigidbody.h"

Rigidbody::Rigidbody(GameObject& gameObject)
	: Component("rigidBody", gameObject)
	, mass(0.f)
	, velocity(0,0,0)
	, gravity(0,0,0)
	, useGravity(false)
	, isKinematic(false)
{
}

Rigidbody::~Rigidbody()
{

}