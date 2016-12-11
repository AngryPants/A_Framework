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
	//Movement
	INPUT_MOVE_LEFT,
	INPUT_MOVE_RIGHT,
	INPUT_MOVE_FORWARD,
	INPUT_MOVE_BACKWARD,

	//Looking
	INPUT_LOOK_LEFT,
	INPUT_LOOK_RIGHT,
	INPUT_LOOK_UP,
	INPUT_LOOK_DOWN,
	
	//Others
	INPUT_SELECT,
	INPUT_QUIT,
	INPUT_RESET,	
	INPUT_RENDER_SPATIAL_PARTITION_ON,
	INPUT_RENDER_SPATIAL_PARTITION_OFF,

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