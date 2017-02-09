#ifndef PAUSE_MENU_SCRIPT_H
#define PAUSE_MENU_SCRIPT_H

#include "../Script/Script.h"

class PauseMenuScript : public Script {

public:
	enum PAUSE_MENU_OPTION {
		PM_OPTION_MAIN_MENU,
		PM_OPTION_RESUME,		
		PM_OPTION_QUIT,

		NUM_PAUSE_MENU_OPTION,
	};

	unsigned int currentOption;
	bool keydown[2];
	GameObject* optionObjects[NUM_PAUSE_MENU_OPTION];

	//Constructor(s) & Destructor
	PauseMenuScript(GameObject& _gameObject);
	virtual ~PauseMenuScript();

	//Interface Function(s)
	virtual void Update(double _deltaTime);

};

#endif