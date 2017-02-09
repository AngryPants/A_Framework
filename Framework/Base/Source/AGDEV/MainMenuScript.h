#ifndef MAIN_MENU_SCRIPT_H
#define MAIN_MENU_SCRIPT_H

#include "../Script/Script.h"

class MainMenuScript : public Script {

public:
	enum MENU_OPTION {
		M_OPTION_START,
		M_OPTION_QUIT,

		NUM_MENU_OPTION,
	};

	unsigned int currentOption;
	GameObject* optionObjects[NUM_MENU_OPTION];

	//Constructor(s) & Destructor
	MainMenuScript(GameObject& _gameObject);
	virtual ~MainMenuScript();

	//Interface Function(s)
	virtual void Update(double _deltaTime);

};

#endif