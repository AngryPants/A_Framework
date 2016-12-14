#include "PlayerPickUpActionScript.h"
#include "../Input/InputManager.h"

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

	}
}