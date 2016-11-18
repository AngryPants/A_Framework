#include "SceneGraph.h"
#include "SceneNode.h"
#include "../GameObject/GameObject.h"

//Constructor(s) & Destructor
SceneGraph::SceneGraph() {
}

SceneGraph::~SceneGraph() {
	if (roots.empty() == false) {
		cout << "Something is wrong with SceneGraph as not all nodes are cleared." << endl;
	}
	if (referenceCount.empty() == false) {
		cout << "Something is wrong with SceneGraph as not all references are cleared." << endl;
	}
	if (referenceCount.empty() && roots.empty()) {
		cout << "SceneGraph closed properly." << endl;
	}
}

//Interface Function(s)
SceneNode* SceneGraph::CreateSceneNode(GameObject& gameObject) {
	map<string, SceneNode*>::iterator mapIter = roots.find(gameObject.GetSpace());
	if (mapIter == roots.end()) {
		SceneNode* rootNode = new SceneNode(gameObject.GetSpace(), nullptr);
		roots.insert(std::pair<string, SceneNode*>(gameObject.GetSpace(), rootNode));
		referenceCount.insert(std::pair<string, int>(gameObject.GetSpace(), 1));
	} else {
		++referenceCount.find(gameObject.GetSpace())->second;
	}
	return new SceneNode(gameObject.GetSpace(), &gameObject);
}

bool SceneGraph::RemoveSceneNode(SceneNode& node) {
	if (&node == roots.find(node.GetSpace())->second) {
		cout << "Cannot Remove Root SceneNode!" << endl;
		return false;
	}
	map<string, int>::iterator mapIter = referenceCount.find(node.GetSpace());
	--mapIter->second;
	if (mapIter->second <= 0) {
		referenceCount.erase(mapIter);
		delete roots.find(node.GetSpace())->second;
		roots.erase(roots.find(node.GetSpace()));
	}
	return true;
}

SceneNode* SceneGraph::GetRootNode(const string& space) {
	map<string, SceneNode*>::iterator mapIter = roots.find(space);
	if (mapIter != roots.end()) {
		return mapIter->second;
	} else {
		return nullptr;
	}
}