#include "PauseMenuScript.h"
#include "../Application/Application.h"
#include "../Input/InputManager.h"
#include "../Component/Physics/Transform.h"
#include "../GameObject/GameObject.h" 
#include "../Scene/SceneManager.h"

//Constructor(s) & Destructor
PauseMenuScript::PauseMenuScript(GameObject& _gameObject) : Script(_gameObject) {
	for (unsigned int i = 0 ; i < static_cast<unsigned int>(NUM_PAUSE_MENU_OPTION); ++i) {
		optionObjects[i] = nullptr;
	}
	currentOption = 0;
	keydown[0] = false;
	keydown[1] = false;
}

PauseMenuScript::~PauseMenuScript() {
}

//Interface Function(s)
void PauseMenuScript::Update(double _deltaTime) {

	if ((InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_UP] || InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_LEFT]) && keydown[0] == false) {
		if (currentOption > 0) {
			--currentOption;
		}
		keydown[0] = true;
	}
	if (!InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_UP] && !InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_LEFT]) {
		keydown[0] = false;
	}

	if ((InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_DOWN] || InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_RIGHT]) && keydown[1] == false) {
		if (currentOption < static_cast<unsigned int>(NUM_PAUSE_MENU_OPTION) - 1) {
			++currentOption;
		}
		keydown[1] = true;
	}
	if (!InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_DOWN] && !InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_RIGHT]) {
		keydown[1] = false;
	}

	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_MENU_SELECT]) {
		switch (currentOption) {
			case PM_OPTION_RESUME: {
				SceneManager::GetInstance().SetActiveScene("Assignment 1 Scene");
				break;
			}
			case PM_OPTION_MAIN_MENU: {
				SceneManager::GetInstance().SetActiveScene("Main Menu");
				break;
			}
			case PM_OPTION_QUIT: {
				Application::GetInstance().Quit();
				break;
			}
			default:
				break;
		}
	}

	for (unsigned int i = 0 ; i < static_cast<unsigned int>(NUM_PAUSE_MENU_OPTION); ++i) {
		if (optionObjects[i] != nullptr) {
			if (i == currentOption) {
				if (optionObjects[i]->GetComponent<Transform>().GetLocalRotation().x < 0.0f) {
					optionObjects[i]->GetComponent<Transform>().Rotate(360 * _deltaTime, 0, 0);
				}
				if (optionObjects[i]->GetComponent<Transform>().GetLocalRotation().x > 0.0f) {
					optionObjects[i]->GetComponent<Transform>().SetLocalRotation(0, 0, 0);
				}
			} else {
				if (optionObjects[i]->GetComponent<Transform>().GetLocalRotation().x > -90.0f) {
					optionObjects[i]->GetComponent<Transform>().Rotate(-360 * _deltaTime, 0, 0);
				}
				if (optionObjects[i]->GetComponent<Transform>().GetLocalRotation().x < -90.0f) {
					optionObjects[i]->GetComponent<Transform>().SetLocalRotation(-90, 0, 0);
				}
			}
		}
	}

}