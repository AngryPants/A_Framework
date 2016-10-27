#include "EntityManager.h"
#include "EntityBase.h"

//Constructor(s) & Destructor
EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
	Clear();
}

// Update all Entities
void EntityManager::Update(const double& deltaTime) {
	

	//Add new Entities 
	AddEntities();

	//Remove Entities
	RemoveEntities();

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

// Add an entity to the AddQueue
void EntityManager::AddEntity(EntityBase& entity)
{
	// Add entities into AddQueue
	addQueue.insert(&entity);
}

// Add an entity to the RemoveQueue
void EntityManager::RemoveEntity(EntityBase& entity)
{
	// Add entities to remove into removeQueue
	removeQueue.insert(&entity);
}

//Add an entity to this EntityManager
void EntityManager::AddEntities() {
	
	if (addQueue.empty())
		return; 

	// Add Queue here
	for (std::set<EntityBase*>::iterator it = addQueue.begin(); it != addQueue.end(); it++)
	{
		entities.push_back(*it);
	}

	// Clear AddQueue here
	addQueue.clear();
}

// Remove an entity from this EntityManager
void EntityManager::RemoveEntities() {
	
	if (removeQueue.empty())
		return;

	// RemoveQueue here
	for (std::set<EntityBase*>::iterator it = removeQueue.begin(); it != removeQueue.end(); it++)
	{
		//Find the entity to remove
		std::vector<EntityBase*>::iterator findIter = std::find(entities.begin(), entities.end(), *it);

		//Delete the entity if found
		if (findIter != entities.end())
		{
			delete *findIter;
			findIter = entities.erase(findIter);
		}
	}

	// Clear RemoveQueue here
	removeQueue.clear();
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