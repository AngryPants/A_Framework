#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SingletonTemplate.h"
#include <map>
#include <set>
#include <type_traits>
#include "Error.h"
#include "Scene.h"

using namespace std;

class SceneManager : public Singleton<SceneManager> {

	friend class Singleton<SceneManager>;

private:
	//Variable(s)
	map<string, Scene*> sceneMap;
	Scene* activeScene;
	Scene* nextScene;
	set<string> removeQueue;

	//Constructor(s) & Destructor
	SceneManager();
	virtual ~SceneManager();

	//Private Function(s)
	Scene* GetScene(const string& sceneName);

public:
	//Interface Function(s)
	void Update(double deltaTime);
	void Render();

	bool CheckIfSceneExist(const string& sceneName);
	template <class Type>
	void CreateScene(const string& sceneName) {
		if (!is_base_of<Scene, Type>::value) {
			Error::ThrowException("Cannot create scene with a non-scene as type.");
		} else if (CheckIfSceneExist(sceneName)) {
			Error::ThrowException("Cannot CreateScene(" + sceneName + ") as another scene with that name already exists.");
		}
		Scene* scenePtr = new Type(sceneName);
		sceneMap.insert(sceneName, scenePtr);
	}
	void RemoveScene(const string& sceneName);
	void SetActiveScene(const string& sceneName);
	Scene* GetActiveScene();
	void RemoveAllScenes();

};

#endif