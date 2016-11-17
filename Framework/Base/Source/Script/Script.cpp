#include "Script.h"
#include "../GameObject/GameObject.h"

//Constructor(s) & Destructor
Script::Script(GameObject& owner) {
	gameObject = &owner;
}

Script::~Script() {
}

//Interface Function(s)
GameObject& Script::GetGameObject() {
	return *gameObject;
}