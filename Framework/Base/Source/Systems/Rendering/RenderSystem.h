#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "SingletonTemplate.h"
#include "../../Others/IDGenerator.h"
#include <string>
#include <bitset>

using namespace std;

class Mesh;

class RenderSystem : public Singleton<RenderSystem> {

	friend class Singleton<RenderSystem>;

private:
	//Constructor(s) & Destructor
	RenderSystem();
	virtual ~RenderSystem();

	//Private Function(s)
	void RenderRecursion(GameObject& gameObject);

	//Debugging Stuff
	Mesh* gridMesh;

public:
	//Interface Function(s)
	virtual void Update(const string& space, double deltaTime);
	virtual void Render(const string& space);
	virtual void RenderUI(const string& space);

	//Debuging Stuff
	virtual void RenderGrid(const string& space);

};

#endif