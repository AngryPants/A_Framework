#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SingletonTemplate.h"
#include <map>
#include <set>
#include <type_traits>
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
	Scene* GetScene(const string& _sceneName);

public:
	//Interface Function(s)
	void Update(double _deltaTime);
	void Render();

	bool CheckIfSceneExist(const string& _sceneName);
	template <class Type>
	void CreateScene(const string& _sceneName) {
		if (!is_base_of<Scene, Type>::value) {
			cout << "Cannot create scene with a non-scene as type." << endl;
			throw exception("Cannot create scene with a non-scene as type.");
		} else if (CheckIfSceneExist(_sceneName)) {
			cout << "Cannot create scene as another scene with the same name already exists." << endl;
			throw exception("Cannot create scene as another scene with the same name already exists.");
		}
		Scene* scenePtr = new Type(_sceneName);
		sceneMap.insert(pair<string, Scene*>(_sceneName, scenePtr));
	}
	void RemoveScene(const string& _sceneName);
	void ExitScene(const string& _sceneName);
	void SetActiveScene(const string& _sceneName);
	Scene* GetActiveScene();
	void RemoveAllScenes();

};

#endif