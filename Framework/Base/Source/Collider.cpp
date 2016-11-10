#include "Collider.h"
#include "GameObject.h"

//Collision Info
GameObject* CollisionInfo::GetGameObject() {
	try {
		*gameObject;
	} catch (std::exception e) {
		cout << e.what() << endl;
		return nullptr;
	}
	return gameObject;
}