#include "EntityManager.h"
#include "EntityBase.h"

//Constructor(s) & Destructor
EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
}

// Update all Entities
void EntityManager::Update(const double& deltaTime) {

	// Update all entities
	std::vector<EntityBase*>::iterator it, end;
	end = entities.end();
	for (it = entities.begin(); it != end; it++)
	{
		(*it)->Update(deltaTime);
	}

	//Clean up Entities that are done
	it = entities.begin();
	while (it != end)
	{
		if ((*it)->IsDestroyed())
		{
			//Delete entity
			delete *it;
			it = entities.erase(it);
		}
		else
		{
			//Move on to the next entity
			++it;
		}
	}

}

//Render all entities
void EntityManager::Render() {

	// Render all entities
	std::vector<EntityBase*>::iterator it, end;
	end = entities.end();
	for (it = entities.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

// Render the UI entites
void EntityManager::RenderUI() {

	// Render all entities UI
	std::vector<EntityBase*>::iterator it, end;
	end = entities.end();
	for (it = entities.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

//Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase& entity) {
	entities.push_back(&entity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase& entity) {
	
	//Find the entity to remove
	std::vector<EntityBase*>::iterator findIter = std::find(entities.begin(), entities.end(), &entity);
	
	//Delete the entity if found
	if (findIter != entities.end())
	{
		delete *findIter;
		findIter = entities.erase(findIter);
		return true;
	}

	// If entity is not found
	return false;
}

// Clear All entities
void EntityManager::Clear() {

	//Find the last entity
	std::vector<EntityBase*>::iterator start = entities.begin();
	std::vector<EntityBase*>::iterator end = entities.end();

	//loop through the list and erase every object regardless
	while (start != end)
	{
		delete *start;
		start = entities.erase(start);
	}
}