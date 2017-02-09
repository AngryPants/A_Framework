#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <unordered_map>
#include "../Source/SingletonTemplate.h"
#include "../Source/PassKey.h"
#include "LuaFile.h"

using std::unordered_map;
class LuaManager : public Singleton<LuaManager>
{
	friend Singleton < LuaManager > ;
public:
	//Create a Lua File
	LuaFile* CreateLua(string name, string luaFilePath);

	//Check if Lua file Exist
	bool HasLuaFile(string name);

	//Get the Specific Lua File
	LuaFile* GetLuaFile(string name);
private:
	LuaManager();
	virtual ~LuaManager();

	//storing a collection of lua
	unordered_map<string, LuaFile*> luaStateMap;
};

#endif