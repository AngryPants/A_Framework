#ifndef GAMEOBJECT_MANAGER_H
#define GAMEOBJECT_MANAGER_H

#include "SingletonTemplate.h"
#include <map>
#include <set>
#include <string>
#include "GameObject.h"

using namespace std;

//A space is simply a string that is used to identity which "bucket" the entity belongs to.

class GameObjectManager : public Singleton<GameObjectManager> {

	friend class Singleton<GameObjectManager>;

private:
	//Variable(s)
	set<GameObject*> addQueue;
	set<GameObject*> removeQueue;
	map<string, set<GameObject*> > goMap;

	//Constructor(s) & Destructor
	GameObjectManager();
	virtual ~GameObjectManager();

	//Private Function(s)
	void AddGameObjects();
	void RemoveGameObjects();

public:
	//Interface Function(s)
	GameObject& CreateGameObject(const string& space, const string& name = "GameObject");
	void RemoveGameObject(GameObject& gameObject);
	void Clear(const string& space);
	void ClearAll();
	void Update();
	void UpdateScripts(const string& space, const double deltaTime);
	set<GameObject*>& GetGameObjects(const string& space);

};

#endif