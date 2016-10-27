#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityBase;
#include "SingletonTemplate.h"
#include <vector>
#include <set>

using std::vector;
using std::set;

class EntityManager : public Singleton<EntityManager>{

	friend Singleton <EntityManager> ;
private:
	//Constructor(s) & Destructor
	EntityManager();
	virtual ~EntityManager();

	//Variable(s)
	vector<EntityBase*> entities;

	//Didnt use this 2 sets
	set<EntityBase*> addQueue;
	set<EntityBase*> removeQueue;

	// Private Add/Remove entities
	void AddEntities();
	void RemoveEntities();

public:
	//Function(s)
	void Update(const double& deltaTime);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase& entity);
	void RemoveEntity(EntityBase& entity);
	void Clear();

};

#endif // ENTITY_MANAGER_H