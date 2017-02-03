#ifndef LUA_INTERFACE_H
#define LUA_INTERFACE_H

#include "../Lua/lua.hpp"
#include "SingletonTemplate.h"
class LuaInterface : public Singleton<LuaInterface>
{
	friend Singleton < LuaInterface >;

public:
	//Initialization of the Lua Interface Class
	bool Init();

	//Run the Lua Interface Class
	void Run();

	//Drop the Lua Interfface Class
	void Drop();

	// Pointer to the Lua State
	lua_State *theLuaState;

	//Custom functions ????
	int getIntValue(const char* varName);

	float getFloatValue(const char* varName);

	void saveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);

	void saveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);


private:
	~LuaInterface(){};

};

#endif