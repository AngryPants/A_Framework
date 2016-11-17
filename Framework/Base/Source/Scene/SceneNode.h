#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "../GameObject/GameObject.h"
#include <set>

using std::set;

class SceneNode {

private:
	//Variable(s)
	GameObject* gameObject;
	SceneNode* rootNode;
	SceneNode* parent;
	set<SceneNode*> children;
	bool isDestroyed;

	//Private Function(s)
	bool CheckIsParent(const SceneNode* nodePtr) const;
	bool RemoveChild(SceneNode& sceneNode);

public:
	//Constructor(s) & Destructor
	SceneNode(GameObject& gameObject, SceneNode* rootNode);
	virtual ~SceneNode(); //DO NOT CALL THE DESTRUCTOR! SERIOUSLY, DON'T! If you wannt delete shit, use the Destroy() function;

	//Interface Function(s)
	const string& GetSpace() const;
	
	//Deletion
	void Destroy();
	bool IsDestroyed() const;

	//GameObject
	GameObject& GetGameObject(); //Get the GameObject for this SceneNode

	//Parent
	bool SetParent(SceneNode& sceneNode);
	bool RemoveParent();
	GameObject* GetParent();

	//Children
	bool AddChild(SceneNode& sceneNode);
	bool DetachChild(SceneNode& sceneNode);
	bool DetachAllChildren();
	bool DestroyChild(SceneNode& sceneNode);
	bool DestroyAllChildren();
	unsigned int GetNumChildren() const;
	
};

#endif