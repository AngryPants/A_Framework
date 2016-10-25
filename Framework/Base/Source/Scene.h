#ifndef SCENE_H
#define SCENE_H

#include <string>

using std::string;

//NOTES:
//No 2 scenes should ever have the same name. Doing so will fuck things up bad. Real bad... Seriously.
//Any subclasses of Scene SHOULD HAVE, and ONLY should have the name as the constructor parameter.

class Scene {

public:
	//Variable(s)
	const string name;

	//Constructor(s) & Destructor
	Scene(const string& name) : name(name) {}
	virtual ~Scene() {}
	
	//Virtual Function(s)
	virtual void Init() = 0;
	virtual void Save() {}
	virtual void Load() {}
	virtual void Update(double deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

};

#endif