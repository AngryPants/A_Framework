#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "SingletonTemplate.h"
#include <map>
#include <string>

class SceneNode;
class GameObject;

using std::map;
using std::string;

class SceneGraph : public Singleton<SceneGraph> {

	friend class Singleton<SceneGraph>;

private:
	//Variable(s)
	map<string, SceneNode*> roots;
	map<string, int> referenceCount;

	//Constructor(s) & Destructor
	SceneGraph();
	virtual ~SceneGraph();

public:
	//Interface Function(s)
	SceneNode* CreateSceneNode(GameObject& gameObject);
	bool RemoveSceneNode(SceneNode& node);
	SceneNode* GetRootNode(const string& space);

};

#endif