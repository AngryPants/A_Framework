#include "SceneNode.h"
#include "SceneGraph.h"
#include "../GameObject/GameObject.h"
#include "../Systems/SpatialPartition/SPSystem.h"

//Constructor(s) & Destructor
SceneNode::SceneNode(const string& space, GameObject* gameObject) {
	this->space = space;
	this->gameObject = gameObject;
	this->isDestroyed = false;
	parent = nullptr;
	SceneNode* rootNode = SceneGraph::GetInstance().GetRootNode(space);
	if (rootNode != nullptr) {
		rootNode->AddChild(*this); //By default our parent is the root node.
	}
}

//DO NOT CALL THE DESTRUCTOR! SERIOUSLY, DON'T! If you wannt delete shit, use the Destroy() function;
SceneNode::~SceneNode() {
	Destroy();
	if (!IsRoot()) {
		SceneGraph::GetInstance().RemoveSceneNode(*this);
	}
}

//Private Function(s)
/*
\brief Check if this sceneNode is one of our parents.

\param sceneNodePtr
The pointer to check if it is a parent.
*/
bool SceneNode::CheckIsParent(const SceneNode* nodePtr) const {
	if (nodePtr == nullptr) { //If it is a nullptr, it isn't our parent.
		return false;
	} else if (nodePtr == this) { //If it is us, it isn;t our parent.
		return false;
	} else if (IsRoot()) { //If we are the root node, then we have no parent.
		return false;
	}else if (nodePtr == this->parent) { //It is our parent.
		return true;
	} else if (parent != nullptr) { //Ask our parent to check his parent to check is parent, to check this parent, to check his parent, to check his...
		return parent->CheckIsParent(nodePtr);
	}
	return false;
}

//Simply, check if this is a child and if it is, just remove it from our list.
//It does not additional check. This is strictly a private function.
bool SceneNode::DetachChild_Private(SceneNode& sceneNode) {
	set<SceneNode*>::iterator setIter = children.find(&sceneNode);
	if (setIter != children.end()) {
		children.erase(setIter);
		return true;
	}
	return false;
}

//Interface Function(s)
const string& SceneNode::GetSpace() const {
	return this->space;
}

GameObject* SceneNode::GetGameObject() {
	return this->gameObject;
}

//Deletion
void SceneNode::Destroy() {
	if (!isDestroyed) {
		isDestroyed = true;
		if (gameObject != nullptr) {
			gameObject->Destroy();
		}
		DestroyAllChildren();
		if (parent != nullptr) {
			parent->DetachChild_Private(*this);
		}
	}
}

bool SceneNode::IsDestroyed() const {
	return this->isDestroyed;
}

//Parent
bool SceneNode::SetParent(SceneNode& sceneNode) {

	if (IsRoot() == false) {
		return sceneNode.AddChild(*this);
	}
	return false;
}

bool SceneNode::DetachParent() {
	if (!IsRoot()) {
		parent->DetachChild(*this);
	}
	return false;
}

SceneNode* SceneNode::GetParent() {
	return this->parent;
}

bool SceneNode::IsRoot() const {
	return this->parent == nullptr;
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
		sceneNode.parent->DetachChild_Private(sceneNode);
	}
	//Now the child needs to add us as a parent.
	sceneNode.parent = this;
	//We need to add the child into our list.
	children.insert(&sceneNode);
	// if adding child is successful, check if parent is not root note, remove child
	if (sceneNode.GetParent() != SceneGraph::GetInstance().GetRootNode(space))
		SpatialPartitionSystem::GetInstance().GetSpatialPartition(sceneNode.GetSpace())->Remove(sceneNode.GetGameObject()->GetID());

	return true;
}

bool SceneNode::DetachChild(SceneNode& sceneNode) {
	if (DetachChild_Private(sceneNode)) { //If this is a child, we remove it from our list.
		sceneNode.parent = SceneGraph::GetInstance().GetRootNode(space); //Set the child's parent to the root node.
		// Add sceneNode back to spatial partition
		SpatialPartitionSystem::GetInstance().GetSpatialPartition(sceneNode.GetSpace())->Add(sceneNode.GetGameObject()->GetID());
		return true;
	}
	return false;
}

bool SceneNode::DetachAllChildren() {
	for (set<SceneNode*>::iterator setIter = children.begin(); setIter != children.end(); ++setIter) {
		SceneNode* nodePtr = *setIter;
		nodePtr->parent = SceneGraph::GetInstance().GetRootNode(space);
	}
	children.clear();
	return true;
}

bool SceneNode::DestroyChild(SceneNode& sceneNode) {
	if (DetachChild_Private(sceneNode)) {
		sceneNode.Destroy();
		return true;
	}
	return false;
}

/*
NOTE: We have to use a while loop because when we call the Destroy() function of our child,
The child will be erased from our children set, causing our setIter to be invalid.
Hence we need to use a while loop and get the begin() instead, in order not to have
to worry about invalid iterators.
*/
bool SceneNode::DestroyAllChildren() {
	/*for (set<SceneNode*>::iterator setIter = children.begin(); setIter != children.end(); ++setIter) {
		SceneNode* nodePtr = *setIter;
		nodePtr->Destroy();
	}
	children.clear();*/
	while (children.empty() == false) {
		SceneNode* nodePtr = *children.begin();
		nodePtr->Destroy();
	}
	return true;
}

set<SceneNode*> SceneNode::GetChildren() {
	return children;
}

unsigned int SceneNode::GetNumChildren() const {
	return children.size();
}