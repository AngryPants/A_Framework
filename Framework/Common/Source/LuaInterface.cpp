#include "LuaInterface.h"
#include <iostream>

using namespace std;

//Allocating and initializing luaInterface's static data member.
//The pointer is allocated but not the object's constructor

//Initialisation of the Lua Interface Class
bool LuaInterface::Init()
{
	bool result = false;

	//1. Create lua state
	theLuaState = lua_open();

	if (theLuaState)
	{
		//2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		//3. Load lua script
		luaL_dofile(theLuaState, "LuaScript//DM2240.lua");

		result = true;
	}

	return result;
}

//Run the Lua Interface Class
void LuaInterface::Run()
{
	if (theLuaState == NULL)
		return;

	//4. Read the variables
	//lua_getGlobal(lua_State* ,const char*)
	lua_getglobal(theLuaState, "title");

	//extract value 
	const char*title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);

	//Display on screen
	cout << title << endl;
	cout << "........................................" << endl;
	cout << "Width of screen : " << width << endl;
	cout << "Height of screen : " << height << endl;
}

//Drop the Lua Interface class
void LuaInterface::Drop()
{
	if (theLuaState)
	{
		//Close lua state
		lua_close(theLuaState);
	}
}

int LuaInterface::getIntValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return lua_tointeger(theLuaState, -1);
}

//Get a float value through the Lua Interface Class
float LuaInterface::getFloatValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return (float)lua_tonumber(theLuaState, -1);
}

// Save an integer value through the Lua Interface Class
void LuaInterface::saveIntValue(const char* varName, const int value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %d\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
	cout << "....................." << endl;
}

//Save a float value through the Lua Interface Class
void LuaInterface::saveFloatValue(const char* varName, const float value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %6.4f\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
}