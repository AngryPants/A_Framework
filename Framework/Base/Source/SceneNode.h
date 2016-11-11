#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "GameObject.h"
#include <set>

using std::set;

class SceneNode {

private:
	//Variable(s)
	string space;
	int id;
	GameObject* gameObject;
	SceneNode* parent;
	set<SceneNode*> children;
	
public:
	
	//Constructor(s) & Destructor
	SceneNode(const string& space);
	virtual ~SceneNode() {}

	//Interface Function(s)
	const string& GetSpace() const;

	void Destroy(); //Destroy this node and it's children.

	void SetID(const int id); //Set the ID for this node.
	int GetID() const; //Get the ID for this node.

	bool SetGameObject(GameObject* gameObject); //Set the GameObject for this SceneNode
	GameObject* GetGameObject(); //Get the GameObject for this SceneNode

	void SetParent(SceneNode* parent);
	SceneNode* GetParent();
	
	//Get the GameObject inside this SceneGraph
	//SceneNode* GetGameObject(GameObject* gameObject);
	//SceneNode* GetGameObject(const int id);

	SceneNode* AddChild(GameObject* child);
	bool DeleteChild(GameObject* child);
	bool DeleteChild(const int id);
	bool DeleteAllChildren();
	SceneNode* DetachChild(GameObject* child);
	SceneNode* DetachChild(const int id);
	int GetNumberOfChildren() const;

};

#endif