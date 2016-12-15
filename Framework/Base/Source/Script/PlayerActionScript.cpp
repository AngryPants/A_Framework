#include "PlayerActionScript.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../Component/Game/Rifle.h"
#include "PlayerShootingScript.h"

PlayerActionScript::PlayerActionScript(GameObject& gameObject) : Script(gameObject)
{
}

PlayerActionScript::~PlayerActionScript()
{
}
 
void PlayerActionScript::OnTriggerStay(const Collider& _collider)
{
	GameObject* temp = GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID);
	if (temp->HasComponent<RifleComponent>()
		&& (temp->GetScript(0) != nullptr && dynamic_cast<PlayerShootingScript*>(temp->GetScript(0)) != nullptr))
	{
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERPICKUP])
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
		else if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERDISCARD] && temp->GetComponent<RifleComponent>().isHeld)
		{
			temp->GetComponent<RifleComponent>().isHeld = false;
			temp->DetachParent();
			//temp->GetComponent<Transform>().SetLocalRotation(0, 0, 0);
		}
	}
}