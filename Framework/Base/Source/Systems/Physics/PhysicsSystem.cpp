#include "PhysicsSystem.h"
#include "CollisionUtility.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../GameObject/GameObject.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Physics/Rigidbody.h"
#include "../../Systems/SpatialPartition/SPSystem.h"
#include "../../Systems/SpatialPartition/Grid.h"

//Include Components
#include "../../Component/Physics/Collision/ColliderGroup.h"

//Include Colliders
#include "../../Collider/AABBCollider.h"
#include "../../Collider/SphereCollider.h"

//Constructor(s) & Destructor
PhysicsSystem::PhysicsSystem() {
}

PhysicsSystem::~PhysicsSystem() {
}

//Collision Helper Functions
void Callback_OnCollisionEnter(GameObject* _goPtr, CollisionInfo* _info) {
	for (unsigned int i = 0; i < MAX_SCRIPTS; ++i) {
		Script* script = _goPtr->GetScript(i);
		if (script != nullptr) {
			script->OnCollisionEnter(*_info);
		}
	}
}

void Callback_OnCollisionStay(GameObject* _goPtr, CollisionInfo* _info) {
	for (unsigned int i = 0; i < MAX_SCRIPTS; ++i) {
		Script* script = _goPtr->GetScript(i);
		if (script != nullptr) {
			script->OnCollisionStay(*_info);
		}
	}
}

void Callback_OnCollisionExit(GameObject* _goPtr, CollisionInfo* _info) {
	for (unsigned int i = 0; i < MAX_SCRIPTS; ++i) {
		Script* script = _goPtr->GetScript(i);
		if (script != nullptr) {
			script->OnCollisionExit(*_info);
		}
	}
}

//Trigger Helper Functions
void Callback_OnTriggerEnter(GameObject* _goPtr, Collider* _otherCollider) {
	for (unsigned int i = 0; i < MAX_SCRIPTS; ++i) {
		Script* script = _goPtr->GetScript(i);
		if (script != nullptr) {
			script->OnTriggerEnter(*_otherCollider);
		}
	}
}

void Callback_OnTriggerStay(GameObject* _goPtr, Collider* _otherCollider) {
	for (unsigned int i = 0; i < MAX_SCRIPTS; ++i) {
		Script* script = _goPtr->GetScript(i);
		if (script != nullptr) {
			script->OnTriggerStay(*_otherCollider);
		}
	}
}

void Callback_OnTriggerExit(GameObject* _goPtr, Collider* _otherCollider) {
	for (unsigned int i = 0; i < MAX_SCRIPTS; ++i) {
		Script* script = _goPtr->GetScript(i);
		if (script != nullptr) {
			script->OnTriggerExit(*_otherCollider);
		}
	}
}

//Private Function(s)
void PhysicsSystem::CheckGrid(Grid& _grid) {
	if (0 == _grid.GetNumObjects()) {
		return;
	}
	//Iterate through the GameObjects.
	vector<GameObjectID> goList = _grid.GetListOfObjects();
	for (vector<GameObjectID>::iterator vecIter = goList.begin(); vecIter != goList.end(); ++vecIter) {
		for (vector<GameObjectID>::iterator vecIter2 = std::next(vecIter); vecIter2 != goList.end(); ++vecIter2) {
			//Get the GameObject.
			GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(*vecIter);
			GameObject* go2 = GameObjectManager::GetInstance().GetGameObjectByID(*vecIter2);
			if (go == nullptr || go2 == nullptr) {
				continue;
			}
			//Check for Colliders.
			SortCollisionType(go, go2);
		}		
	}
}

void PhysicsSystem::CheckGrid(Grid& _grid, Grid& _grid2) {
	if (0 == _grid.GetNumObjects() || 0 == _grid2.GetNumObjects()) {
		return;
	}

	const vector<GameObjectID>& goList = _grid.GetListOfObjects();
	const vector<GameObjectID>& goList2 = _grid2.GetListOfObjects();
	for (vector<GameObjectID>::const_iterator vecIter = goList.begin(); vecIter != goList.end(); ++vecIter) {
		for (vector<GameObjectID>::const_iterator vecIter2 = goList2.begin(); vecIter2 != goList2.end(); ++vecIter2) {
			//Get the GameObject.
			GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(*vecIter);
			GameObject* go2 = GameObjectManager::GetInstance().GetGameObjectByID(*vecIter2);
			if (go == nullptr || go2 == nullptr) {
				continue;
			}
			//Check for Colliders.
			SortCollisionType(go, go2);
		}
	}
}

void PhysicsSystem::SortCollisionType(GameObject* _a, GameObject* _b) {
	if (_a->HasActiveComponent<ColliderGroup<SphereCollider>>()) {
		if (_b->HasActiveComponent<ColliderGroup<SphereCollider>>()) {
			Collision_Sphere_Sphere(_a, _b);
		}
	}
}

/*void PhysicsSystem::Collision_AABB_AABB(GameObject* _a, GameObject* _b) {
	Transform& aTransform = _a->GetComponent<Transform>();
	ColliderGroup<AABBCollider>& aColliderGroup = _a->GetComponent<ColliderGroup<AABBCollider>>();

	Transform& bTransform = _b->GetComponent<Transform>();
	ColliderGroup<AABBCollider>& bColliderGroup = _b->GetComponent<ColliderGroup<AABBCollider>>();

	//Another nested for loop to check every AABB Collider.
	for (vector<AABBCollider>::iterator aIter = aColliderGroup.colliders.begin(); aIter != aColliderGroup.colliders.end(); ++aIter) {
		for (vector<AABBCollider>::iterator bIter = bColliderGroup.colliders.begin(); bIter != aColliderGroup.colliders.end(); ++bIter) {

		}
	}
}

void PhysicsSystem::Collision_AABB_Sphere(GameObject* _a, GameObject* _b) {
	Transform& aTransform = _a->GetComponent<Transform>();
	ColliderGroup<AABBCollider>& aColliderGroup = _a->GetComponent<ColliderGroup<AABBCollider>>();

	Transform& bTransform = _b->GetComponent<Transform>();
	ColliderGroup<SphereCollider>& bColliderGroup = _b->GetComponent<ColliderGroup<SphereCollider>>();
}*/

void PhysicsSystem::Collision_Sphere_Sphere(GameObject* _a, GameObject* _b) {
	ColliderGroup<SphereCollider>& aColliderGroup = _a->GetComponent<ColliderGroup<SphereCollider>>();
	if (!aColliderGroup.IsActive()) {
		return;
	}
	ColliderGroup<SphereCollider>& bColliderGroup = _b->GetComponent<ColliderGroup<SphereCollider>>();
	if (!bColliderGroup.IsActive()) {
		return;
	}

	Transform& aTransform = _a->GetComponent<Transform>();
	Transform& bTransform = _b->GetComponent<Transform>();
	//Another nested for loop to check every Sphere Collider.
	for (vector<SphereCollider>::iterator aIter = aColliderGroup.colliders.begin(); aIter != aColliderGroup.colliders.end(); ++aIter) {
		//Collider
		SphereCollider& aCollider = *aIter;
		if (!aCollider.isActive) {
			continue;
		}
		
		for (vector<SphereCollider>::iterator bIter = bColliderGroup.colliders.begin(); bIter != bColliderGroup.colliders.end(); ++bIter) {
			//Collider
			SphereCollider& bCollider = *bIter;
			if (!bCollider.isActive) {
				continue;
			}

			//GameObjects
			GameObject* aGO = GameObjectManager::GetInstance().GetGameObjectByID(aCollider.gameObjectID);
			GameObject* bGO = GameObjectManager::GetInstance().GetGameObjectByID(bCollider.gameObjectID);

			//Positions
			Vector3 aPosition = aGO->GetComponent<Transform>().GetPosition() + aCollider.centre;
			Vector3 bPosition = bGO->GetComponent<Transform>().GetPosition() + bCollider.centre;

			//If either one is a trigger, then do empirical check.
			if (aCollider.isTrigger || bCollider.isTrigger || (!aGO->HasActiveComponent<Rigidbody>() && !bGO->HasActiveComponent<Rigidbody>())) {
				if (CollisionUtility::CheckCollisionSphereSphere_Empirical(aPosition, aCollider.GetRadius(), bPosition, bCollider.GetRadius())) {
					Callback_OnTriggerStay(aGO, &bCollider);
					Callback_OnTriggerStay(bGO, &aCollider);
				}
			} else if (aGO->HasActiveComponent<Rigidbody>() && bGO->HasActiveComponent<Rigidbody>()) {
				//Mathematical Check
				/*Rigidbody& aRigidbody = aGO->GetComponent<Rigidbody>();
				Rigidbody& bRigidbody = bGO->GetComponent<Rigidbody>();

				ContactPoint contactPoint(&aCollider, &bCollider);
				//Check for collision
				if (CollisionUtility::CheckCollisionSphereSphere_Mathematical(
					aPosition, aRigidbody.velocity, aCollider.GetRadius(),
					bPosition, bRigidbody.velocity, bCollider.GetRadius(),
					contactPoint.point, contactPoint.normal, contactPoint.separation)) {

					//Now that there's a collision,
					//Inform A
					CollisionInfo aInfo;
					aInfo.contacts.push_back(contactPoint);
					aInfo.gameObject = bGO->GetID();
					aInfo.relativeVelocity = aRigidbody.velocity - bRigidbody.velocity;
					Callback_OnCollisionStay(aGO, &aInfo);

					//Inform B
					CollisionInfo bInfo;					
					bInfo.contacts.push_back(contactPoint);
					//Flip the normal around.
					bInfo.contacts.back().normal.Flip();
					bInfo.gameObject = aGO->GetID();
					bInfo.relativeVelocity = bRigidbody.velocity - aRigidbody.velocity;
					Callback_OnCollisionStay(bGO, &bInfo);

					//Collision Response
					Vector3 uNormalA = aRigidbody.velocity.Projection(contactPoint.normal); //The component of Ball A's velocity along the normal.
					Vector3 uNormalB = bRigidbody.velocity.Projection(contactPoint.normal); //The component of Ball B's velocity along the normal.

					float averageElasticity = (aRigidbody.elasticity + bRigidbody.elasticity) * 0.5f;
					float combinedMass = aRigidbody.GetMass() + bRigidbody.GetMass();

					aRigidbody.velocity = averageElasticity * (aRigidbody.velocity + (2.0f * bRigidbody.GetMass() / combinedMass) * (uNormalB - uNormalA));
					bRigidbody.velocity = averageElasticity * (bRigidbody.velocity + (2.0f * aRigidbody.GetMass() / combinedMass) * (uNormalA - uNormalB));
				}*/
				Response_Sphere_Sphere(&aCollider, &aGO->GetComponent<Rigidbody>(), aPosition,
									   &bCollider, &bGO->GetComponent<Rigidbody>(), bPosition);
			} else if (aGO->HasActiveComponent<Rigidbody>() && !bGO->HasActiveComponent<Rigidbody>()) {
				Response_Sphere_Sphere(&aCollider, &aGO->GetComponent<Rigidbody>(), aPosition,
									   &bCollider, bGO, bPosition);
			} else if (!aGO->HasActiveComponent<Rigidbody>() && bGO->HasActiveComponent<Rigidbody>()) {
				Response_Sphere_Sphere(&bCollider, &bGO->GetComponent<Rigidbody>(), bPosition,
									   &aCollider, aGO, aPosition);
			}
		}
	}
}

void PhysicsSystem::Response_Sphere_Sphere(SphereCollider* _colliderA, Rigidbody* _rigidbodyA, const Vector3& _positionA, SphereCollider* _colliderB, Rigidbody* _rigidbodyB, const Vector3& _positionB) {

	ContactPoint contactPoint(_colliderA, _colliderB);
	//Check for collision
	if (CollisionUtility::CheckCollisionSphereSphere_Mathematical(
		_positionA, _rigidbodyA->velocity, _colliderA->GetRadius(),
		_positionB, _rigidbodyB->velocity, _colliderB->GetRadius(),
		deltaTime,
		contactPoint.point, contactPoint.normal, contactPoint.separation)) {

		GameObject* aGO = &_rigidbodyA->GetGameObject();
		GameObject* bGO = &_rigidbodyB->GetGameObject();

		//Now that there's a collision,
		//Inform A
		CollisionInfo aInfo;
		aInfo.contacts.push_back(contactPoint);
		aInfo.gameObject = bGO->GetID();
		aInfo.relativeVelocity = _rigidbodyA->velocity - _rigidbodyB->velocity;
		Callback_OnCollisionStay(aGO, &aInfo);

		//Inform B
		CollisionInfo bInfo;					
		bInfo.contacts.push_back(contactPoint);
		//Flip the normal around.
		bInfo.contacts.back().normal.Flip();
		bInfo.gameObject = aGO->GetID();
		bInfo.relativeVelocity = _rigidbodyB->velocity - _rigidbodyA->velocity;
		Callback_OnCollisionStay(bGO, &bInfo);

		//Collision Response
		Vector3 uNormalA = _rigidbodyA->velocity.Projection(contactPoint.normal); //The component of Ball A's velocity along the normal.
		Vector3 uNormalB = _rigidbodyB->velocity.Projection(contactPoint.normal); //The component of Ball B's velocity along the normal.

		float averageElasticity = (_rigidbodyA->elasticity + _rigidbodyB->elasticity) * 0.5f;
		float combinedMass = _rigidbodyA->GetMass() + _rigidbodyB->GetMass();

		_rigidbodyA->velocity = averageElasticity * (_rigidbodyA->velocity + (2.0f * _rigidbodyB->GetMass() / combinedMass) * (uNormalB - uNormalA));
		_rigidbodyB->velocity = averageElasticity * (_rigidbodyB->velocity + (2.0f * _rigidbodyA->GetMass() / combinedMass) * (uNormalA - uNormalB));
	}
}

void PhysicsSystem::Response_Sphere_Sphere(SphereCollider* _colliderA, Rigidbody* _rigidbodyA, const Vector3& _positionA, SphereCollider* _colliderB, GameObject* _goB, const Vector3& _positionB) {
	GameObject* goA = &_rigidbodyA->GetGameObject();

	ContactPoint contactPoint(_colliderA, _colliderB);
	//Check for collision
	if (CollisionUtility::CheckCollisionSphereSphere_Mathematical(
		_positionA, _rigidbodyA->velocity, _colliderA->GetRadius(),
		_positionB, Vector3(0, 0, 0), _colliderB->GetRadius(),
		deltaTime,
		contactPoint.point, contactPoint.normal, contactPoint.separation)) {

		//Now that there's a collision,
		//Inform A
		CollisionInfo aInfo;
		aInfo.contacts.push_back(contactPoint);
		aInfo.gameObject = _goB->GetID();
		aInfo.relativeVelocity = _rigidbodyA->velocity;
		Callback_OnCollisionStay(goA, &aInfo);

		//Inform B
		CollisionInfo bInfo;					
		bInfo.contacts.push_back(contactPoint);
		//Flip the normal around.
		bInfo.contacts.back().normal.Flip();
		bInfo.gameObject = goA->GetID();
		bInfo.relativeVelocity = -_rigidbodyA->velocity;
		Callback_OnCollisionStay(_goB, &bInfo);

		//Collision Response
		Vector3 uNormalA = _rigidbodyA->velocity.Projection(contactPoint.normal); //The component of Ball A's velocity along the normal.
		(_rigidbodyA->velocity -= 2.0f * uNormalA) *= _rigidbodyA->elasticity;
	}
}

//Function(s)
void PhysicsSystem::UpdateDeltaTime(const string& _space, double _deltaTime) {
	this->deltaTime = _deltaTime;
	set<Component*> rigidbodies = ComponentManager::GetInstance().GetComponents<Rigidbody>(_space);
	for (set<Component*>::iterator setIter = rigidbodies.begin(); setIter != rigidbodies.end(); ++setIter) {
		Rigidbody* componentPtr = static_cast<Rigidbody*>(*setIter);
		componentPtr->SetDeltaTime(_deltaTime, {});
	}
}

void PhysicsSystem::UpdateGravity(const string& _space) {
	set<Component*> rigidbodies = ComponentManager::GetInstance().GetComponents<Rigidbody>(_space);
	for (set<Component*>::iterator setIter = rigidbodies.begin(); setIter != rigidbodies.end(); ++setIter) {
		Rigidbody* componentPtr = static_cast<Rigidbody*>(*setIter);
		if (componentPtr->IsActive() && componentPtr->useGravity) {
			componentPtr->AddRelativeForce(componentPtr->gravity, FORCE_MODE::FM_ACCELERATION);
		}
	}
}

void PhysicsSystem::UpdateCollision(const string& _space) {
	SpatialPartition* sp = SpatialPartitionSystem::GetInstance().GetSpatialPartition(_space);
	
	for (int x = 0; x < sp->GetxNumOfGrid(); ++x) {
		for (int y = 0; y < sp->GetyNumOfGrid(); ++y) {
			for (int z = 0; z < sp->GetzNumOfGrid(); ++z) {
				//continue;
				//Get the grid. If there is nothing in the grid, continue;
				Grid& grid = sp->GetGrid(x, y, z);
				if (grid.GetNumObjects() == 0) {
					continue;
				}
				CheckGrid(grid);				

				//Check with the surrounding grids.
				for (unsigned int i = 0; i < 2; ++i) {
					for (unsigned int j = 0; j < 2; ++j) {
						for (unsigned int k = 0; k < 2; ++k) {
							int xIndex = x + i;
							int yIndex = y + j;
							int zIndex = z + k;
							//If it is the original grid, ignore it.
							if (xIndex == x && yIndex == y && zIndex == z) {
								continue;
							}
							//If it is not within our entire spatial partition, ignore it
							if (xIndex >= sp->GetxNumOfGrid() ||
								yIndex >= sp->GetyNumOfGrid() ||
								zIndex >= sp->GetzNumOfGrid()) {
								continue;
							}
							CheckGrid(grid, sp->GetGrid(xIndex, yIndex, zIndex));
						}
					}
				}

				//Check with the extra grid.
				CheckGrid(sp->GetGrid(x, y, z), sp->GetExtraGrid());
			}
		}
	}
	
	//Check the extra grid.
	CheckGrid(sp->GetExtraGrid());
}

void PhysicsSystem::UpdateMovement(const string& _space) {
	set<Component*> rigidbodies = ComponentManager::GetInstance().GetComponents<Rigidbody>(_space);
	for (set<Component*>::iterator setIter = rigidbodies.begin(); setIter != rigidbodies.end(); ++setIter) {
		Rigidbody* componentPtr = static_cast<Rigidbody*>(*setIter);
		if (componentPtr->IsActive()) {
			Transform& transform = componentPtr->GetGameObject().GetComponent<Transform>();
			transform.Translate(componentPtr->velocity * deltaTime);
		}
	}
}

void PhysicsSystem::Update(const string& _space) {
	UpdateGravity(_space);
	UpdateCollision(_space);
	UpdateMovement(_space);
}