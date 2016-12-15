#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../Others/IDGenerator.h"
#include "../Component/ComponentManager.h"
#include "../Component/Physics/Transform.h"
#include "../Script/Script.h"
#include "../Scene/SceneGraph.h"
#include "../Scene/SceneNode.h"
#include "PassKey.h"
#include <string>
#include <bitset>
#include <exception>
#include <vector>
#include <set>
#include <map>

using namespace std;

typedef std::bitset<MAX_COMPONENTS> ComponentBitset;

class GameObjectManager;

static const int MAX_SCRIPTS = 8;

class GameObject {

private:
	//Variable(s)
	GameObjectID id;
	string space;
	ComponentBitset componentBitset;
	Component* components[MAX_COMPONENTS];
	SceneNode* node;
	Script* scripts[MAX_SCRIPTS];
	bool destroyed;
	
	//Destructor
	virtual ~GameObject();

public:
	//Constructor(s)
	GameObject(const string& _space, const string& _name, PassKey<GameObjectManager> _key);	
	void Delete(PassKey<GameObjectManager> _key);

	//Variable(s)
	string name;
	set<string> tags;
	
	//Name
	void SetName(const string& name);
	const string& GetName() const;
	const string& GetSpace() const;
	int GetID() const;

	//Components
	template <class Type>
	bool HasComponent() const {
		return componentBitset[IDGenerator::GetInstance().GetComponentTypeID<Type>()];
	}

	template <class Type>
	bool HasActiveComponent() const {
		if (!HasComponent<Type>()) {
			return false;
		}

		return GetComponent<Type>().IsActive();
	}

	template <class Type>
	Type& AddComponent() {
		ComponentTypeID id = IDGenerator::GetInstance().GetComponentTypeID<Type>();
		Component* componentPtr = nullptr;
		if (HasComponent<Type>()) {
			cout << "This GameObject already has this component." << endl;
			componentPtr = components[id];
			//throw std::exception("This GameObject already has this component.");
		} else {
			componentBitset[id] = 1;
			componentPtr = &ComponentManager::GetInstance().CreateComponent<Type>(*this);
			components[id] = componentPtr;
		}		
		
		return *(static_cast<Type*>(componentPtr));
	}

	template <class Type>
	void RemoveComponent() {
		if (!HasComponent<Type>()) {
			cout << "This GameObject does not have this component." << endl;
			throw std::exception("This GameObject does not have this component.");
			return;
		}

		ComponentTypeID id = IDGenerator::GetInstance().GetComponentTypeID<Type>();
		componentBitset[id] = 0;
		ComponentManager::GetInstance().RemoveComponent(*components[id],id);
		components[id] = nullptr;
	}

	template<>
	void RemoveComponent<Transform>() {
		cout << "Cannot remove Transform Component. All GameObjects must have it." << endl;
	}

	template <class Type>
	Type& GetComponent() {
		if (!HasComponent<Type>()) {
			cout << "This GameObject does not have this component." << endl;
			throw std::exception("This GameObject does not have this component.");
		}

		Component* componentPtr = components[IDGenerator::GetInstance().GetComponentTypeID<Type>()];
		return *(static_cast<Type*>(componentPtr));
	}

	template <class Type>
	const Type& GetComponent() const {
		if(!HasComponent<Type>()) {
			cout << "This GameObject does not have this component." << endl;
			throw std::exception("This GameObject does not have this component.");
		}

		Component* componentPtr = components[IDGenerator::GetInstance().GetComponentTypeID<Type>()];
		return *(static_cast<Type*>(componentPtr));
	}

	const ComponentBitset& GetComponentBitset() const;

	//Destruction
	void Destroy();
	bool IsDestroyed() const;

	//Scripts
	bool HasScript(unsigned int index);

	template <class Type>
	Type* CreateScript(unsigned int index) {
		if (index > sizeof(scripts)/sizeof(scripts[0]) - 1) {
			string errorMessage = "Unable to AddScript(" + to_string(index) + ") to GameObject " + name + " as there the specified slot is invalid.";
			cout << errorMessage << endl;
			return nullptr;
		}
		if (scripts[index] != nullptr) {
			string errorMessage = "Unable to CreateScript(" + to_string(index) + ") to GameObject " + name + " as there is already a script in the specified slot.";
			cout << errorMessage << endl;
			return nullptr;
		}
		Type* script = new Type(*this);
		this->scripts[index] = script;
		return script;
	}

	template <class Type>
	Type* CreateScript() {
		for (unsigned int i = 0; i < sizeof(scripts)/sizeof(scripts[0]); ++i) {
			if (scripts[i] == nullptr) {
				Type* script = new Type(*this);
				this->scripts[i] = script;
				return script;
			}
		}

		cout << "Unable to CreateScript() as there are no available script slots left." << endl;
		return nullptr;
	}

	void RemoveScript(unsigned int index);
	Script* GetScript(unsigned int index);
	void UpdateScripts(double deltaTime, PassKey<GameObjectManager> _key);

	//Parent
	bool SetParent(GameObject& gameObject);
	bool DetachParent();
	bool HasParent() const;
	bool IsChildOf(const GameObject& gameObject);
	GameObject* GetParent();

	//Child
	bool AddChild(GameObject& gameObject);
	bool DetachChild(GameObject& gameObject);
	bool DetachAllChildren();
	bool DestroyChild(GameObject& gameObject);
	bool DestroyAllChildren();
	void GetChildren(vector<GameObject*>& children);

};

#endif