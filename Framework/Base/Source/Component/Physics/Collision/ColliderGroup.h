#ifndef COLLIDER_GROUP_H
#define COLLIDER_GROUP_H

#include "../../Component.h"
#include "../../../Collider/Collider.h"
#include "../../../GameObject/GameObject.h"
#include <vector>
#include <map>

using std::vector;
using std::map;

template <class Type>
class ColliderGroup : public Component {

public:
	//Variable(s)
	vector<Type> colliders;

	//Constructor(s) & Destructor
	ColliderGroup(GameObject& _gameObject, PassKey<ComponentManager> _key) : Component("Collider Group", _gameObject, _key) {
		if (!std::is_base_of<Collider, Type>::value) {
			string errorMessage = "Cannot create ColliderGroup of Type that is not a Collider.";
			cout << errorMessage << endl;
	        throw std::exception(errorMessage.c_str());
		}

		CreateColliders(1);
	}
	virtual ~ColliderGroup() {}
	
	//Function(s)
	void CreateColliders(unsigned int numColliders = 1) {
		if (!std::is_base_of<Collider, Type>::value) {
			string errorMessage = "Cannot use CreateCollider() on non-collider.";
			cout << errorMessage << endl;
	        throw std::exception(errorMessage.c_str());
		}

		for (unsigned int i = 0; i < numColliders; ++i) {
			colliders.emplace_back(GetGameObject().GetID());
		}
	}

};

#endif