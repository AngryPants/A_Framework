#include "PlayerPickUpActionScript.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../Component/Game/Rifle.h"

PlayerPickUpActionScript::PlayerPickUpActionScript(GameObject& gameObject) : Script(gameObject)
{
}

PlayerPickUpActionScript::~PlayerPickUpActionScript()
{
}
 
void PlayerPickUpActionScript::OnTriggerStay(const Collider& _collider)
{
	GameObject* temp = GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID);
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERPICKUP] && temp->HasComponent<RifleComponent>())
	{
		vector<GameObject*> children;
		GetGameObject().GetChildren(children);
		for (vector<GameObject*>::iterator iter = children.begin(); iter != children.end(); iter++)
		{
			if ((*iter)->name == "Player Camera")
			{
				temp->SetParent(**iter);
				temp->GetComponent<Transform>().SetLocalPosition(-0.5, -0.5, 0.5);
				temp->GetComponent<RifleComponent>().isHeld = true;
				break;
			}
		}
	}


}