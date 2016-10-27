#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityBase;
//#include "EntityBase.h"
#include <vector>
#include <set>

using std::vector;
using std::set;

class EntityManager {

private:
	//Variable(s)
	vector<EntityBase*> entities;

	//Didnt use this 2 sets
	set<EntityBase*> addQueue;
	set<EntityBase*> removeQueue;

public:
	//Constructor(s) & Destructor
	EntityManager();
	virtual ~EntityManager();

	//Function(s)
	void Update(const double& deltaTime);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase& entity);
	bool RemoveEntity(EntityBase& entity);
	void Clear();

};

#endif // ENTITY_MANAGER_H