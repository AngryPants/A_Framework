#include "SceneNode.h"

//Constructor(s) & Destructor
SceneNode::SceneNode(const string& space) : space(space) {
	id = -1;
	this->gameObject = nullptr;
	this->parent = nullptr;
}

SceneNode::~SceneNode() {
}

//Interface Function(s)
const string& SceneNode::GetSpace() const {
	return this->space;
}

void SceneNode::Destroy() {
}

void SceneNode::SetID(const int id) {
	this->id = id;
}

int SceneNode::GetID() const {
	return this->id;
}

bool SceneNode::SetGameObject(GameObject* gameObject) {
	if (gameObject->GetSpace() != this->space) {
		cout << "A SceneNode can only take in a GameObject in the same space as it." << endl;
		throw std::exception("A SceneNode can only take in a GameObject in the same space as it.");
	}
	this->gameObject = gameObject;
	return true;
}

GameObject* SceneNode::GetGameObject() {
	return this->gameObject;
}

void SceneNode::SetParent(SceneNode* parent) {
	this->parent = parent;
}

SceneNode* SceneNode::GetParent() {
	return this->parent;	
}

SceneNode* SceneNode::AddChild(GameObject* child) {
	return nullptr;
}

bool SceneNode::DeleteChild(GameObject* child) {
	return false;
}

bool SceneNode::DeleteChild(const int id) {
	return false;
}

bool SceneNode::DeleteAllChildren() {
	for (set<SceneNode*>::iterator setIter = children.begin(); setIter != children.end(); ++setIter) {
		SceneNode* sceneNodePtr = *setIter;
		sceneNodePtr->Destroy();
	}
	
	return true;
}

SceneNode* SceneNode::DetachChild(GameObject* child) {
	return nullptr;
}

SceneNode* SceneNode::DetachChild(const int id) {
	return nullptr;
}

int SceneNode::GetNumberOfChildren() const {
	return children.size();
}