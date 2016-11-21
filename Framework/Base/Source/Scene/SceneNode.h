#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <set>
#include <string>

using std::set;
using std::string;

class GameObject;

class SceneNode {

private:
	//Variable(s)
	GameObject* gameObject;
	SceneNode* parent;
	set<SceneNode*> children;
	bool isDestroyed;
	string space;

	//Private Function(s)
	bool CheckIsParent(const SceneNode* nodePtr) const;
	bool DetachChild_Private(SceneNode& sceneNode);

public:
	//Constructor(s) & Destructor
	SceneNode(const string& space, GameObject* gameObject);
	virtual ~SceneNode(); //DO NOT CALL THE DESTRUCTOR! SERIOUSLY, DON'T! If you wannt delete shit, use the Destroy() function.

	//Interface Function(s)
	const string& GetSpace() const;
	
	//Deletion
	void Destroy();
	bool IsDestroyed() const;

	//GameObject
	GameObject* GetGameObject(); //Get the GameObject for this SceneNode

	//Parent
	bool SetParent(SceneNode& sceneNode);
	bool DetachParent();
	SceneNode* GetParent();
	bool IsRoot() const;

	//Children
	bool AddChild(SceneNode& sceneNode);
	bool DetachChild(SceneNode& sceneNode);
	bool DetachAllChildren();
	bool DestroyChild(SceneNode& sceneNode);
	bool DestroyAllChildren();
	set<SceneNode*> GetChildren();
	unsigned int GetNumChildren() const;
	
};

#endif