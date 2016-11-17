#include "SceneNode.h"

//Constructor(s) & Destructor
SceneNode::SceneNode(GameObject& gameObject, SceneNode* rootNode) {
	this->rootNode = rootNode;
	this->parent = rootNode;
	this->gameObject = nullptr;	
	this->isDestroyed = false;
}

//DO NOT CALL THE DESTRUCTOR! SERIOUSLY, DON'T! If you wannt delete shit, use the Destroy() function;
SceneNode::~SceneNode() {
}

//Private Function(s)
/*
\brief Check if this sceneNode is one of our parents.

\param sceneNodePtr
The pointer to check if it is a parent.
*/
bool SceneNode::CheckIsParent(const SceneNode* nodePtr) const {
	if (nodePtr == nullptr) {
		return false;
	} else if (nodePtr == this->parent) {
		return true;
	} else if (parent != nullptr) {
		return parent->CheckIsParent(nodePtr);
	}
	return false;
}

//Simply, check if this is a child and if it is, just remove it from our list.
//It does not additional check. This is strictly a private function.
bool SceneNode::RemoveChild(SceneNode& sceneNode) {
	set<SceneNode*>::iterator setIter = children.find(&sceneNode);
	if (setIter != children.end()) {
		children.erase(setIter);
		return true;
	}
	return false;
}

//Interface Function(s)
const string& SceneNode::GetSpace() const {
	return this->gameObject->GetSpace();
}

GameObject& SceneNode::GetGameObject() {
	return *this->gameObject;
}

//Deletion
void SceneNode::Destroy() {
	if (!isDestroyed) {
		isDestroyed = true;
		gameObject->Destroy();
		DestroyAllChildren();
		parent->RemoveChild(*this);
	}	
}

bool SceneNode::IsDestroyed() const {
	return this->isDestroyed;
}

//Parent
bool SceneNode::SetParent(SceneNode& sceneNode) {
	return sceneNode.AddChild(*this);
}

bool SceneNode::RemoveParent() {
	return false;
}

GameObject* SceneNode::GetParent() {
	return nullptr;
}

//Children
bool SceneNode::AddChild(SceneNode& sceneNode) {
	//We need to do a few layers of checking.
	//Let's make sure it isn't ourselves.
	if (this == &sceneNode) {
		return false;
	}
	//Make sure we aren't destroyed.
	if (sceneNode.IsDestroyed() || this->IsDestroyed()) {
		cout << "Unable to AddChild to SceneNode as one of them is already destroyed." << endl;
		return false;
	}
	//Let's make sure that the space is the same.
	if (sceneNode.GetSpace() != this->GetSpace()) {
		cout << "When adding a SceneNode parent or child, both nodes must belong to the same space." << endl;
		return false;
	}
	//We need to make sure that it isn't one of our parents.
	if (CheckIsParent(&sceneNode)) {
		return false;
	}
	//Lastly, we need to make sure that if it already has a parent, it detaches it.
	if (sceneNode.parent != nullptr) {
		sceneNode.parent->RemoveChild(sceneNode);
	}
	//Now the child needs to add us as a parent.
	sceneNode.parent = this;
	//We need to add the child into our list.
	children.insert(&sceneNode);
	return true;
}

bool SceneNode::DetachChild(SceneNode& sceneNode) {
	if (RemoveChild(sceneNode)) {
		sceneNode.parent = rootNode;
		return true;
	}
	return false;
}

bool SceneNode::DetachAllChildren() {
	for (set<SceneNode*>::iterator setIter = children.begin(); setIter != children.end(); ++setIter) {
		SceneNode* nodePtr = *setIter;
		nodePtr->parent = rootNode;
	}
	children.clear();
	return true;
}

bool SceneNode::DestroyChild(SceneNode& sceneNode) {
	if (RemoveChild(sceneNode)) {
		sceneNode.Destroy();
		return true;
	}
	return false;
}

bool SceneNode::DestroyAllChildren() {
	for (set<SceneNode*>::iterator setIter = children.begin(); setIter != children.end(); ++setIter) {
		SceneNode* nodePtr = *setIter;
		nodePtr->Destroy();
	}
	children.clear();
	return true;
}

unsigned int SceneNode::GetNumChildren() const {
	return children.size();
}