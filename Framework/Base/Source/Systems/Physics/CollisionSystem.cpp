#include "CollisionSystem.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Physics/Transform.h"
#include "../../GameObject/GameObject.h"
#include "../../Component/Colliders/BoxCollider.h"
#include "../../Component/Colliders/SphereCollider.h"
#include "../../Component/Physics/Rigidbody.h"


/*float CollisionSystem::CircleCircle(Vector2 circlePositionA, Vector2 circlePositionB, float circleRadiusA, float circleRadiusB, Vector2 circleVelocityA, Vector2 circleVelocityB, bool directionCheck)
{
Vector2 relativeVelocity = circleVelocityA - circleVelocityB;
Vector2 relativeDirection = circlePositionA - circlePositionB; //Usually we do B - A but for this case A - B makes calculations easier.
float combinedRadius = circleRadiusA + circleRadiusB;

if (directionCheck && relativeVelocity.Dot(relativeDirection) > 0) {
return -1; //We the 2 circles aren't going towards each other.
}

float a = relativeVelocity.LengthSquared();
float b = 2 * relativeVelocity.Dot(relativeDirection);
float c = relativeDirection.LengthSquared() - combinedRadius * combinedRadius;
float d = b * b - 4 * a * c;

float timeToCollision = (-b - sqrt(d)) / (2 * a);
if (timeToCollision < 0)
timeToCollision = (-b + sqrt(d)) / (2 * a);

return timeToCollision;

}

float CollisionSystem::LineCircle(Vector2 linePosition, Vector2 circlePosition, float circleRadius, Vector2 lineVelocity, Vector2 circleVelocity, bool directionCheck)
{
return CircleCircle(linePosition, circlePosition, 0, circleRadius, lineVelocity, circleVelocity, directionCheck);
}*/

// Trigger Check only Happens when
/*
0. The Object must be Active (of course)
1. The Object must have a Collider (of course x2)
2. The Other Rigidbody it collides with must fufill (0 && 1) as well

*/

// Trigger Response
/*

*/

// Collision Check only Happens when
/*
0. The Object must be Active (of course)
1. The Object Must have a RigidBody
2. The RigidBody must have a Collider
3. The RigidBody must be awake (to show that it will hit something, something else hitting it doesnt count)
4. The Other Rigidbody it collides with must fufill (0 && 1 && 2) as well
5. Both Object must not be kinematic
6. Both Objects collider must not be a trigger
*/

// Collision Response
/*
1. All affected by collision will check, if their velocity > certain amount, set it to awake
*/

void CollisionSystem::CheckCollision(const string& space, double deltaTime) {

	set<Component*>& rigidBodies = ComponentManager::GetInstance().GetComponents<Rigidbody>(space);

	// First loop to check first GameObject
	for (set<Component*>::iterator setIter = rigidBodies.begin(); setIter != rigidBodies.end(); ++setIter) {
		if (!(*setIter)->isActive)
		{
			continue;
		}
		Rigidbody* rigidbody = static_cast<Rigidbody*>(*setIter);
		if (rigidbody->IsSleeping() || !rigidbody->GetGameObject().HasComponent<Collider>() || rigidbody->isKinematic || rigidbody->GetGameObject().GetComponent<Collider>().isTrigger)
		{
			continue;
		}

		// Second loop to check second GameObject
		for (set<Component*>::iterator setIterTwo = next(setIter, 1); setIterTwo != rigidBodies.end(); ++setIterTwo) {
			if (!(*setIterTwo)->isActive)
			{
				continue;
			}

			Rigidbody* rigidbody2 = static_cast<Rigidbody*>(*setIter);
			if (!rigidbody->GetGameObject().HasComponent<Collider>() || rigidbody->isKinematic || rigidbody->GetGameObject().GetComponent<Collider>().isTrigger)
			{
				continue;
			}

			// Collision check HERE <TERRY HELP CODE HERE ONEGAISHIMASU!>
			/*Transform& transformOne = rigidbody->GetGameObject().GetComponent<Transform>();
			Transform& transformTwo = rigidbody2->GetGameObject().GetComponent<Transform>();

			float lengthSquared = (transformOne.GetPosition() - transformTwo.GetPosition()).LengthSquared();
			float combinedRadSq = (sphereCollider->GetRadius() + sphereCollider2->GetRadius()) * (sphereCollider->GetRadius() + sphereCollider2->GetRadius());

			if (lengthSquared < combinedRadSq) {
				sphereCollider->info.Collide(sphereCollider2->GetGameObject(), transformOne.GetPosition());
				sphereCollider2->info.Collide(sphereCollider->GetGameObject(), transformTwo.GetPosition());
			}*/
			//
			// 2
			/*float lengthSquared = (sphereCollider->centre - sphereCollider2->centre).LengthSquared();
			float combinedRadSq = (sphereCollider->GetRadius() + sphereCollider2->GetRadius()) * (sphereCollider->GetRadius() + sphereCollider2->GetRadius());

			if (lengthSquared < combinedRadSq) {
			sphereCollider->info.Collide(sphereCollider->GetGameObject(), sphereCollider->centre);
			sphereCollider2->info.Collide(sphereCollider2->GetGameObject(), sphereCollider2->centre);
			}*/
		}
	}
}

// Incomplete, not priority 
void CollisionSystem::CheckTrigger(const string& space, double deltaTime)
{
	set<Component*>& rigidBodies = ComponentManager::GetInstance().GetComponents<Rigidbody>(space);

	// First loop to check first GameObject
	for (set<Component*>::iterator setIter = rigidBodies.begin(); setIter != rigidBodies.end(); ++setIter) {
		if (!(*setIter)->isActive)
		{
			continue;
		}
		Rigidbody * rigidbody = static_cast<Rigidbody*>(*setIter);
		if (!rigidbody->GetGameObject().HasComponent<Collider>() || !rigidbody->GetGameObject().GetComponent<Collider>().isTrigger)
			continue;

		// Second loop to check second GameObject
		for (set<Component*>::iterator setIterTwo = next(setIter, 1); setIterTwo != rigidBodies.end(); ++setIterTwo) {
			if (!(*setIterTwo)->isActive)
			{
				continue;
			}
			// Collision check HERE <TERRY HELP CODE HERE ONEGAISHIMASU!>
		}
	}
}

void CollisionSystem::Reset(const string& space) {
	set<Component*>& sphereColliders = ComponentManager::GetInstance().GetComponents<SphereCollider>(space);

	// First loop to check first GameObject
	for (set<Component*>::iterator setIter = sphereColliders.begin(); setIter != sphereColliders.end(); ++setIter) {
		SphereCollider* sphereCollider = static_cast<SphereCollider*>(*setIter);
		sphereCollider->info.Reset();
	}
}

void CollisionSystem::UpdateAllRigidBodies(const string& space)
{
	set<Component*>& rigidbodies = ComponentManager::GetInstance().GetComponents<Rigidbody>(space);
	for (set<Component*>::iterator setIter = rigidbodies.begin(); setIter != rigidbodies.end(); ++setIter) {
		Rigidbody * rigidbody = static_cast<Rigidbody*>(*setIter);
		if (rigidbody->GetVelocity().IsZero())
			rigidbody->Sleep();
		else
			rigidbody->WakeUp();
	}
}