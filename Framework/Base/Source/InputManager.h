#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <cstdlib>

#include "SingletonTemplate.h"
#include <bitset>
#include <climits>
#include "MyMath.h"

using std::bitset;

//Some Example Controls
enum INPUT_TYPE {
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_FORWARD,
	INPUT_BACKWARD,

	INPUT_QUIT,
	INPUT_RESET,
	
	NUM_KEYS,
};

struct InputInfo {

public:
	//Variable(s)
	bitset<NUM_KEYS> keyDown;
	bitset<NUM_KEYS> keyPressed;
	bitset<NUM_KEYS> previousState;
	bitset<NUM_KEYS> keyReleased;
	float keyValue[NUM_KEYS];

	//Constructor(s) & Destructor
	InputInfo() = default;
	virtual ~InputInfo() = default;

	void ClampValues() {
		for (auto& value : keyValue) {
			value = Math::Clamp(value, 0.0f, 1.0f);
		}
	}

	void Reset() {
		keyDown.reset();
		keyReleased.set();
		keyPressed.reset(); 
		//previousState.reset();
		for (unsigned int i = 0; i < static_cast<unsigned int>(NUM_KEYS); ++i) {
			keyValue[i] = 0.0f;
		}
	}

	void Update() {
		for (std::size_t i = 0; i < NUM_KEYS; ++i) {
			if (keyDown[i] && !previousState[i]) {
				previousState[i] = true;
				keyPressed[i] = true;
			}
			else if (keyReleased[i]) {
				previousState[i] = false;
				keyPressed[i] = false;
			}
			else {
				keyPressed[i] = false;
			}
		}

		ClampValues(); //This is to ensure that our keyValues are between 0.0f and 1.0f;
	}

};

//The job of the InputManager is to translate all the devices input into the inputs that will be used in the game.
//Therefore the InputManager will need to be edited for every game.

class InputManager : public Singleton<InputManager> {

	friend class Singleton<InputManager>;

private:
	//Variable(s)
	InputInfo inputInfo;

	//Constructor(s) & Destructor
	InputManager();
	virtual ~InputManager();

public:
	//Function(s)
	const InputInfo& GetInputInfo() const;
	void Update();

};

#endif