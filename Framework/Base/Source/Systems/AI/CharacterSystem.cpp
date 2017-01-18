#include "CharacterSystem.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Component/AI/CharacterComponent.h"
#include <iostream>
using std::cout;
using std::endl;

CharacterManager::CharacterManager()
	:isUpdating(false)
{
}

CharacterManager::~CharacterManager()
{
	AllCharacters.clear();
	addQueue.clear();
	removeQueue.clear();

}

void CharacterManager::AddCharacter(const GameObject& character)
{
	if (!character.HasActiveComponent<CharacterComponent>())
	{
		cout << "character you're Adding Must have Character Component" << endl;
		return;
	}

	for (set<GameObjectID>::iterator iter = AllCharacters.begin(); iter != AllCharacters.end(); iter++)
	{
		if (&character == GameObjectManager::GetInstance().GetGameObjectByID(*iter))
		{
			cout << "Cannot AddObserver(Character& observer) self." << endl;
			return;
		}
	}

	if (!isUpdating)
		AllCharacters.insert(character.GetID());
	else
		addQueue.insert(character.GetID());
}

void CharacterManager::RemoveCharacter(const GameObject& character)
{
	if (!isUpdating)
	{
		set<GameObjectID>::iterator setIter = AllCharacters.find(character.GetID());
		if (setIter != AllCharacters.end())
			AllCharacters.erase(setIter);
	}
	else
		removeQueue.insert(character.GetID());
}

void CharacterManager::Update(const double dt)
{
	isUpdating = true;

	//Add new chara
	for (set<GameObjectID>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ++setIter) {
		AllCharacters.insert(*setIter);
	}
	addQueue.clear();

	//remove new chara
	for (set<GameObjectID>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		set<GameObjectID>::iterator removeIter = AllCharacters.find(*setIter);
		if (removeIter != AllCharacters.end()) {
			AllCharacters.erase(removeIter);
		}
	}
	removeQueue.clear();

	// Remove all character that died
	set<GameObjectID>::iterator iter = AllCharacters.begin();
	set<GameObjectID>::iterator end = AllCharacters.end();
	while (iter != end)
	{
		if (GameObjectManager::GetInstance().GetGameObjectByID(*iter)->IsDestroyed())
		{
			// Delete if done
			iter = AllCharacters.erase(iter);
		}
		else
		{
			// Move on otherwise
			++iter;
		}
	}

	isUpdating = false;
}




