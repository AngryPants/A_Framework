#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityBase;
#include "SingletonTemplate.h"
#include <vector>
#include <set>
#include <map>

using std::vector;
using std::set;
using std::map;
using std::pair;

class EntityManager : public Singleton<EntityManager> {

	friend class Singleton <EntityManager>;
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