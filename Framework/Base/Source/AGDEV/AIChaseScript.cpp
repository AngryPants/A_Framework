#include "AIChaseScript.h"
#include "../GameObject/GameObjectManager.h"
#include "../Component/Physics/Rigidbody.h"
#include "../Collider/Collider.h"
#include "../GameObject/GameObjectFactory.h"

//constructor and destructor
AIChaseScript::AIChaseScript(GameObject& gameObject) 
	: Script(gameObject)
	, playerPosition(0,0,0)
{
	hasPlayer = false;
}

AIChaseScript::~AIChaseScript()
{
}

void AIChaseScript::OnTriggerStay(const Collider& _collider)
{
	/*if (&_collider == &GetGameObject().GetComponent<ColliderGroup<SphereCollider>>().colliders[3])
	{*/
		GameObject* temp = GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID);
		if (temp->name == "Player")
		{
			playerPosition = temp->GetComponent<Transform>().GetPosition();
			hasPlayer = true;
		}
	//}
}

//Function(s)
void AIChaseScript::Update(double deltaTime)
{
	if (hasPlayer)
	{
		Vector3 direction = ((playerPosition - GetGameObject().GetComponent<Transform>().GetPosition()).Normalized());
		GetGameObject().GetComponent<Rigidbody>().AddRelativeForce(direction * 1000);
	}
	hasPlayer = false;
}