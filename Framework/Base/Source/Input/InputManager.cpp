/******************************************************************************/
/*!
\file	InputManager.cpp
\author	Muhammad Shafik Bin Mazlinan
\par	email: cyboryxmen\@yahoo.com
\brief
Contains the implementation of the input manager.

*/
/******************************************************************************/

#include "InputManager.h"

//Input Devices
#include "Controller_Keyboard.h"
#include "Controller_Mouse.h"
#include "Controller_Gamepad.h"
#include <iostream>

using namespace controller;

InputManager::InputManager() {
	inputInfo.Reset();
}

InputManager::~InputManager() {
	Mouse::Destroy();
	Keyboard::Destroy();
	GamepadManager::Destroy();
}

const InputInfo& InputManager::GetInputInfo() const {
	return this->inputInfo;
}

void SetInput(InputInfo& inputInfo, const std::size_t index, KEY_STATE state = KEY_STATE::PRESS, float value = 1.0f) {
	switch (state) {
		case KEY_STATE::RELEASE: {		
			inputInfo.keyDown[index] = false;
			inputInfo.keyReleased[index] = true;
			inputInfo.keyValue[index] = 0.0f;	
		}
		break;
		case KEY_STATE::PRESS: {
			inputInfo.keyDown[index] = true;
			inputInfo.keyReleased[index] = false;
			inputInfo.keyValue[index] = value;	
		}
		break;
	}
}

void ProcessInput(InputInfo& inputInfo, const Key& input) {
	/*********************************************************************************************************/
	//This is where we start customising the controls for our game. It might get a little tedious.
	//For the sake of standardisation, ensure that keyValue is between 0.0f to 1.0f.
	//The point of keyValue is when using mouse or controller. When using a mouse, moving the mouse from the centre of
	//the screen all the way to the edge of the screen will be considered 1.0f.
	/*********************************************************************************************************/

	//Quit Button
	switch (input.GetKey()) {
		//Utility
		case KEYS::ESCAPE: {
			SetInput(inputInfo, INPUT_QUIT, input.GetState());
			break;
		}
		case KEYS::R: {
			SetInput(inputInfo, INPUT_RESET, input.GetState());
			break;
		}
		case KEYS::ENTER: {
			SetInput(inputInfo,INPUT_SELECT,input.GetState());
			break;
		}

		//Movement
		case KEYS::W: {
			SetInput(inputInfo,INPUT_MOVE_FORWARD,input.GetState());
			break;
		}
		case KEYS::S: {
			SetInput(inputInfo,INPUT_MOVE_BACKWARD,input.GetState());
			break;
		}
		case KEYS::A: {
			SetInput(inputInfo, INPUT_MOVE_LEFT, input.GetState());
			break;
		}
		case KEYS::D: {
			SetInput(inputInfo, INPUT_MOVE_RIGHT, input.GetState());
			break;
		}

		//Looking
		case KEYS::LEFT: {
			SetInput(inputInfo,INPUT_LOOK_LEFT,input.GetState());
			break;
		}
		case KEYS::RIGHT: {
			SetInput(inputInfo,INPUT_LOOK_RIGHT,input.GetState());
			break;
		}
		case KEYS::UP: {
			SetInput(inputInfo,INPUT_LOOK_UP,input.GetState());
			break;
		}
		case KEYS::DOWN: {
			SetInput(inputInfo,INPUT_LOOK_DOWN,input.GetState());
			break;
		}
		case KEYS::Q: {
			SetInput(inputInfo,INPUT_ROLL_LEFT,input.GetState());
			break;
		}
		case KEYS::E: {
			SetInput(inputInfo,INPUT_ROLL_RIGHT,input.GetState());
			break;
		}
	}
}

void ProcessInput(InputInfo& inputInfo, const Gamepad& input) {

	/****************************************/
	//This is where we start customising the controls for our game. It might get a little tedious.
	//For the sake of standardisation, ensure that keyValue is between 0.0f to 1.0f.
	//The point of keyValue is when using mouse or controller. When using a mouse, moving the mouse from the centre of
	//the screen all the way to the edge of the screen will be considered 1.0f.
	/****************************************/

	/*
	float left_horizontal_axis = input.axes[0]; //Left Thumb Stick
	//Check if the stick was moved.
	if (std::abs(left_horizontal_axis) > GamepadManager::GetInstance().GetDeadzone()) {
		//Check which direction it was moved.
		if(left_horizontal_axis < 0) { //Left
			SetInput(inputInfo, INPUT_MOVE_LEFT, KEY_STATE::PRESS, std::abs(left_horizontal_axis));
			} else if(left_horizontal_axis > 0) { //Right
			SetInput(inputInfo, INPUT_MOVE_RIGHT, KEY_STATE::PRESS, std::abs(left_horizontal_axis));
			}
	 }

	//Back
	if (input.buttons[6]) {
		SetInput(inputInfo, INPUT_PAUSE, KEY_STATE::PRESS);
	}

	//A
	if (input.buttons[0] == true) {
		SetInput(inputInfo, INPUT_JUMP, KEY_STATE::PRESS);
		SetInput(inputInfo, INPUT_SELECT, KEY_STATE::PRESS);
	}

	//B
	if (input.buttons[1]) {
		SetInput(inputInfo, INPUT_BACK, KEY_STATE::PRESS);
	}

	//X
	if (input.buttons[2] == true) {
		SetInput(inputInfo, INPUT_SHOOT, KEY_STATE::PRESS);
	}

	//Y
	if (input.buttons[3] == true) {
		SetInput(inputInfo, INPUT_ABILITY, KEY_STATE::PRESS);
	}

	//Back
	if (input.buttons[6] == true) {
		//SetInput(inputInfo, INPUT_QUIT, KEY_STATE::PRESS);
	}

	//D-Pad Up
	if(input.buttons[10] == true) {
		SetInput(inputInfo, INPUT_MENU_UP, KEY_STATE::PRESS);
	}

	//D-Pad Right
	if(input.buttons[11] == true) {
		SetInput(inputInfo, INPUT_MENU_RIGHT, KEY_STATE::PRESS);
	}

	//D-Pad Down
	if(input.buttons[12] == true) {
		SetInput(inputInfo, INPUT_MENU_DOWN, KEY_STATE::PRESS);
	}

	//D-Pad Left
	if(input.buttons[13] == true) {
		//SetInput(inputInfo, INPUT_MOVE_LEFT, KEY_STATE::PRESS);
		SetInput(inputInfo, INPUT_MENU_LEFT, KEY_STATE::PRESS);
	}
	*/

}

void InputManager::Update() {

	Keyboard& keyboard = Keyboard::GetInstance();
	Mouse& mouse = Mouse::GetInstance();
	GamepadManager& gamepadManager = GamepadManager::GetInstance();

	mouse.ReadInput();
	gamepadManager.UpdateJoysticks();

	//Process keyboard input.
	bool hasGamepad = false;
	for (auto& gamepad : gamepadManager.GetGamepads()) {
		if (gamepad.active) {
			hasGamepad = true;
			inputInfo.Reset();
			ProcessInput(inputInfo, gamepad);
			break;
		}
	}

	if (!hasGamepad) {
		try {
			while (true) {
				ProcessInput(inputInfo, keyboard.GetKey());
			}
		} catch (std::exception exception) {
			//queue empty. Moving on.
			//std::cout << "No keys pressed this update." << std::endl;
		}
	} else {
		keyboard.ClearInput();
	}
	/****************************************/
	//Do not touch anything below unless you know what you're doing.
	/****************************************/

	inputInfo.Update();
}