#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "../../GameObject/GameObject.h"
#include "SingletonTemplate.h"
#include <set>
#include <string>
#include <map>

using std::map;
using std::string;
using std::set;

class CharacterManager : public Singleton<CharacterManager>
{
	friend Singleton < CharacterManager >;
private:

	set<GameObjectID> AllCharacters;
	set<GameObjectID> addQueue;
	set<GameObjectID> removeQueue;
	CharacterManager();
	virtual ~CharacterManager();

	bool isUpdating;
public:

	//Interface Function
	void AddCharacter(const GameObject& character);
	void RemoveCharacter(const GameObject& character);

	void Update(const double dt);
};


#endif