#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "SingletonTemplate.h"
#include "../Others/IDGenerator.h"
#include <string>
#include <bitset>

using namespace std;

class RenderSystem : public Singleton<RenderSystem> {

	friend class Singleton<RenderSystem>;

private:
	//Constructor(s) & Destructor
	RenderSystem();
	virtual ~RenderSystem();

public:
	//Interface Function(s)
	virtual void Update(const string& space, double deltaTime);
	virtual void Render(const string& space);
	virtual void RenderUI(const string& space);

};

#endif