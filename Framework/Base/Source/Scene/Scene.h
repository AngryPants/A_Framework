#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "PassKey.h"

using std::string;

//NOTES:
//No 2 scenes should ever have the same name. Doing so will fuck things up bad. Real bad... Seriously.
//Any subclasses of Scene SHOULD HAVE, and ONLY should have the name as the constructor parameter.

class SceneManager;

class Scene {

private:
	bool initialised;

protected:
	virtual void Init() = 0;
	virtual void Update(double deltaTime) = 0;
	virtual void Render() = 0;	
	virtual void Exit() = 0;

public:
	//Variable(s)
	const string name;

	//Constructor(s) & Destructor
	Scene(const string& name) : name(name) {
		initialised = false;
	}
	virtual ~Scene() {}
	
	//Virtual Function(s)
	void Init(PassKey<SceneManager> _key) {
		if (!initialised) {
			Init();
			initialised = true;
		}
	}
	void Update(double _deltaTime, PassKey<SceneManager> _key) {
		if (initialised) {
			Update(_deltaTime);
		}		
	}
	void Render(PassKey<SceneManager> _key) {
		if (initialised) {
			Render();
		}
	}
	void Exit(PassKey<SceneManager> _key) {
		if (initialised) {
			Exit();
			initialised = false;			
		}
	}
	bool IsInitialised() const {
		return initialised;
	}

};

#endif