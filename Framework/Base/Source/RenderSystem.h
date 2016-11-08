#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "SingletonTemplate.h"
#include "IDGenerator.h"
#include <string>
#include <bitset>

using namespace std;

class RenderSystem : public Singleton<RenderSystem> {

	friend class Singleton<RenderSystem>;

private:
	//Variable(s)
	std::bitset<MAX_COMPONENTS> componentBitset;

	//Constructor(s) & Destructor
	RenderSystem();
	virtual ~RenderSystem();

public:
	//Interface Function(s)
	virtual void Render(const string& space);

};

#endif