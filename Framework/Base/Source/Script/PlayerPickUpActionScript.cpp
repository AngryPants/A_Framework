#include "PlayerPickUpActionScript.h"
#include "../Input/InputManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/Colliders/BoxCollider.h"
PlayerPickUpActionScript::PlayerPickUpActionScript(GameObject& gameObject) : Script(gameObject)
{
}

PlayerPickUpActionScript::~PlayerPickUpActionScript()
{
}
 
void PlayerPickUpActionScript::Update(double deltaTime)
{
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERPICKUP])
	{
		GetGameObject().GetComponent<BoxCollider>().OnTriggerEnter();
		{
			// item pick up here
		}
	}
}