#include "GameObject.h"
#include "GameObjectManager.h"

//Constructor(s) & Destructor
GameObject::GameObject(const string& _space, const string& _name, PassKey<GameObjectManager> _key)
: space(_space), name(_name) {
	destroyed = false;
	id = IDGenerator::GetInstance().GenerateGameObjectID({});
	for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
		this->components[i] = nullptr;
	}
	for (unsigned int i = 0; i < sizeof(scripts) / sizeof(scripts[0]); ++i) {
		scripts[i] = nullptr;
	}
	tags.insert("Default");
	AddComponent<Transform>();
	node = SceneGraph::GetInstance().CreateSceneNode(*this); //SceneGraph
	useSpatialPartition = true;
}

GameObject::~GameObject() 
{
	for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
		if (this->components[i] != nullptr) {
			ComponentManager::GetInstance().RemoveComponent(*components[i], i);
		}
	}
	componentBitset.reset();
	for (unsigned int i = 0; i < sizeof(scripts) / sizeof(scripts[0]); ++i) {
		if (scripts[i] != nullptr) {
			delete scripts[i];
		}
	}
	IDGenerator::GetInstance().ReturnGameObjectID(id, {});
	delete node; //SceneGraph
}

void GameObject::Delete(PassKey<GameObjectManager> _key) {
	delete this;
}

//Name
void GameObject::SetName(const string& name) {
	this->name = name;
}
const string& GameObject::GetName() const {
	return this->name;
}
const string& GameObject::GetSpace() const {
	return this->space;
}
int GameObject::GetID() const {
	return this->id;
}

const ComponentBitset& GameObject::GetComponentBitset() const {
	return this->componentBitset;
}

//Destruction
void GameObject::Destroy() {
	if (destroyed == false) {
		destroyed = true;
		GameObjectManager::GetInstance().RemoveGameObject(*this, {});
		node->Destroy();
	}	
}
bool GameObject::IsDestroyed() const {
	return destroyed;
}

//Scripts
Script* GameObject::GetScript(unsigned int index) {
	if (index > sizeof(scripts) / sizeof(scripts[0]) - 1) {
		string errorMessage = "Unable to GetScript(" + to_string(index) + ") of GameObject " + name + " as there the specified slot is invalid.";
		cout << errorMessage << endl;
		return nullptr;
	}

	return scripts[index];
}

void GameObject::RemoveScript(unsigned int index) {
	if (index > sizeof(scripts) / sizeof(scripts[0]) - 1) {
		string errorMessage = "Unable to RemoveScript(" + to_string(index) + ") of GameObject " + name + " as there the specified slot is invalid.";
		cout << errorMessage << endl;
		return;
	}
	if (scripts[index] != nullptr) {
		delete scripts[index];
		scripts[index] = nullptr;
	}
}
bool GameObject::HasScript(unsigned int index) {
	if (index > sizeof(scripts) / sizeof(scripts[0]) - 1) {
		return false;
	}
	return scripts[index] != nullptr;
}

void GameObject::UpdateScripts(double deltaTime, PassKey<GameObjectManager> _key) {
	for (unsigned int i = 0; i < sizeof(scripts)/sizeof(scripts[0]); ++i) {
		if (scripts[i] != nullptr) {
			scripts[i]->Update(deltaTime);
		}
	}
}

//Parent-Child
bool GameObject::SetParent(GameObject& gameObject) {
	if (node->SetParent(*gameObject.node)) {
		GetComponent<Transform>().SetLocalPosition(0, 0, 0);
		return true;
	}
	return false;
}

bool GameObject::DetachParent() {
	return node->DetachParent();
}
bool GameObject::HasParent() const {
	return node->GetParent() != nullptr;
}
bool GameObject::IsChildOf(const GameObject& gameObject) {
	return (node->GetParent() == gameObject.node);
}
GameObject* GameObject::GetParent() {
	return node->GetParent()->GetGameObject();
}

//Child
bool GameObject::AddChild(GameObject& gameObject) {
	return node->AddChild(*gameObject.node);
}
bool GameObject::DetachChild(GameObject& gameObject) {
	return node->DetachChild(*(gameObject.node));
}
bool GameObject::DetachAllChildren() {
	return node->DetachAllChildren();
}
bool GameObject::DestroyChild(GameObject& gameObject) {
	return node->DestroyChild(*(gameObject.node));
}
bool GameObject::DestroyAllChildren() {
	return node->DestroyAllChildren();
}
void GameObject::GetChildren(vector<GameObject*>& children) {
	children.clear();
	children.resize(node->GetNumChildren(), nullptr);
	set<SceneNode*>& childrenNodes = node->GetChildren();
	int i = 0;
	for (set<SceneNode*>::iterator setIter = childrenNodes.begin(); setIter != childrenNodes.end(); ++setIter) {
		children[i] = (*setIter)->GetGameObject();
		++i;
	}
}