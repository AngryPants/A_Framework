#include "LuaManager.h"

#include <iostream>
using std::cout;
using std::endl;
using std::pair;
using std::string;

LuaManager::LuaManager()
{
	luaStateMap.clear();
}

LuaManager::~LuaManager()
{
	map<string, LuaFile*>::iterator iter = luaStateMap.begin();
	while (iter != luaStateMap.end())
	{
		delete ((*iter).second);
		iter = luaStateMap.erase(iter);
	}
	luaStateMap.clear();
}

LuaFile* LuaManager::CreateLua(string name, string luaFilePath)
{
	//1. Create lua state
	LuaFile * newLuaFile;
	newLuaFile = new LuaFile(/*{}*/);

	if (newLuaFile->Init(luaFilePath))
	{
		if (HasLuaFile(name) && GetLuaFile(name) != nullptr)
		{
			cout << "Replacing Lua File at :" + name << endl;
		}

		luaStateMap.insert(pair<string, LuaFile*>(name, newLuaFile));
		cout << "Successfully Created Lua File at :" + name + " FilePath : " + luaFilePath << endl;
		return newLuaFile;
	}
	else
	{
		cout << "Unsuccessful at creating Lua File name :" + name + " FilePath : " + luaFilePath  + " is the file path correct ?" << endl;
	}

	return nullptr;
}
		 
bool LuaManager::HasLuaFile(string name)
{
	return luaStateMap.count(name) != 0;
}

LuaFile* LuaManager::GetLuaFile(string name)
{
	if (!HasLuaFile(name))
	{
		cout << "No such file name , are you sure you've typed in the right name : " + name<<endl;
		return nullptr;
	}

	return luaStateMap.find(name)->second;
}