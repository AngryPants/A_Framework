#include "CollisionSystem.h"
#include "../Component/ComponentManager.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Physics/Collision/SphereCollider.h"
#include "../GameObject/GameObject.h"

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

void CollisionSystem::CheckCollision(const string& space, double deltaTime) {

	set<Component*>& sphereColliders = ComponentManager::GetInstance().GetComponents<SphereCollider>(space);

	// First loop to check first GameObject
	for (set<Component*>::iterator setIter = sphereColliders.begin(); setIter != sphereColliders.end(); ++setIter) {
		SphereCollider* sphereCollider = static_cast<SphereCollider*>(*setIter);
		if (!sphereCollider->isActive || sphereCollider->GetGameObject().HasComponent<Transform>() == false) {
			continue;
		}
		// Second loop to check second GameObject
		for (set<Component*>::iterator setIterTwo = next(setIter, 1); setIterTwo != sphereColliders.end(); ++setIterTwo) {
			SphereCollider* sphereCollider2 = static_cast<SphereCollider*>(*setIterTwo);

			if (!sphereCollider2->isActive || sphereCollider2->GetGameObject().HasComponent<Transform>() == false) {
				continue;
			}
			// Collision check
			// 1
			Transform& transformOne = sphereCollider->GetGameObject().GetComponent<Transform>();
			Transform& transformTwo = sphereCollider2->GetGameObject().GetComponent<Transform>();

			float lengthSquared = (transformOne.GetPosition() - transformTwo.GetPosition()).LengthSquared();
			float combinedRadSq = (sphereCollider->GetRadius() + sphereCollider2->GetRadius()) * (sphereCollider->GetRadius() + sphereCollider2->GetRadius());

			if (lengthSquared < combinedRadSq) {
				sphereCollider->info.Collide(sphereCollider2->GetGameObject(), transformOne.GetPosition());
				sphereCollider2->info.Collide(sphereCollider->GetGameObject(), transformTwo.GetPosition());
			}
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

void CollisionSystem::Reset(const string& space) {
	set<Component*>& sphereColliders = ComponentManager::GetInstance().GetComponents<SphereCollider>(space);

	// First loop to check first GameObject
	for (set<Component*>::iterator setIter = sphereColliders.begin(); setIter != sphereColliders.end(); ++setIter) {
		SphereCollider* sphereCollider = static_cast<SphereCollider*>(*setIter);
		sphereCollider->info.Reset();
	}
}