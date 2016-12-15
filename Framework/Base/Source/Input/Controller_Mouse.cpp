#include "Controller_Mouse.h"
#include "../Application/Application.h"
#include "MyMath.h"
#include <iostream>

using std::cout;
using std::endl;

namespace controller {

	Mouse::Mouse() {
		window = nullptr;
		deadZone = 0.01f;
		travelDistanceX = 0.0f;
		travelDistanceY = 0.0f;
	}

	Mouse::~Mouse() {
	}

	void Mouse::SetWindow(GLFWwindow*const window) {
		this->window = window;
	}

	bool Mouse::IsKeyPressed(unsigned int key) { //0 - Left, 1 - Right, 2 - Middle
		if (key < 0 || key >= MAX_KEYS) {
			return false;
		}
		return currentState[key];
	}

	bool Mouse::IsKeyReleased(unsigned int key) {
		if (key < 0 || key >= MAX_KEYS) {
			return false;
		}
		if (currentState[key] == 0 && previousState[key] == 1) {
			return true;
		}
		return false;
	}

	CursorPosition Mouse::GetCursorPosition() {
		return this->cursorPosition;
	}

	float Mouse::GetTravelDistanceX() const {
		return travelDistanceX;
	}

	//NOTE: THE 0 VALUE FOR Y IS AT THE TOP OF THE SCREEN.
	float Mouse::GetTravelDistanceY() const {
		return travelDistanceY;
	}

	void Mouse::SetDeadZone(const float& deadZone) {
		if (deadZone < 0.0f) {
			this->deadZone = 0.0f;
		} else if (deadZone > 1.0f) {
			this->deadZone = 1.0f;
		} else {
			this->deadZone = deadZone;
		}
	}

	float Mouse::GetDeadZone() const {
		return this->deadZone;
	}

	void Mouse::ReadInput(bool resetCursor) {
		//Mouse Clicks
		for (int i = 0; i < MAX_KEYS; ++i) {
			previousState[i] = currentState[i];
		}
		currentState.reset();

		for (int j = 0; j < MAX_KEYS; ++j) {
			if (glfwGetMouseButton(window, j) == 0) {
				currentState[j] = 0;
			} else {
				currentState[j] = 1;
			}
		}

		//Mouse Movement
		glfwGetCursorPos(window, &cursorPosition.x, &cursorPosition.y);

		int windowWidth = 0;
		int windowHeight = 0;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		CursorPosition windowCentre;
		windowCentre.Set(static_cast<float>(windowWidth / 2), static_cast<float>(windowHeight / 2));
		if (resetCursor) {
			glfwSetCursorPos(window, windowCentre.x, windowCentre.y);
		}

		//cout << "X (Centre): " << windowCentre.x << " Y (Centre): " << windowCentre.y << endl;
		//cout << "X (Current): " << cursorPosition.x << " Y (Current): " << cursorPosition.y << endl;

		travelDistanceX = static_cast<float>(cursorPosition.x - windowCentre.x) / static_cast<float>(windowWidth / 2);
		travelDistanceY = static_cast<float>(cursorPosition.y - windowCentre.y) / static_cast<float>(windowHeight / 2);

		if (travelDistanceX < deadZone && travelDistanceX > -deadZone) {
			travelDistanceX = 0.0f;
		}
		if (travelDistanceY < deadZone && travelDistanceY > -deadZone) {
			travelDistanceY = 0.0f;
		}

		travelDistanceX = Math::Clamp(travelDistanceX, -1.0f, 1.0f);
		travelDistanceY = Math::Clamp(travelDistanceY, -1.0f, 1.0f);		
	}

	void Mouse::Reset() {
		previousState.reset();
		currentState.reset();
	}

}