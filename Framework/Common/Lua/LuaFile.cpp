#include "LuaFile.h"
#include "LuaManager.h"
#include <iostream>

using namespace std;

//Allocating and initializing luaInterface's static data member.
//The pointer is allocated but not the object's constructor

LuaFile::LuaFile(PassKey<LuaManager> _key)
	: theLuaState(nullptr)
{
}

LuaFile::~LuaFile()
{
	Drop();
}

//Initialisation of the Lua Interface Class
bool LuaFile::Init(string filePath)
{
	bool result = false;

	//1. Create lua state
	theLuaState = lua_open();
	if (theLuaState)
	{
		//2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		//3. Load lua script
		luaL_dofile(theLuaState, filePath.c_str());

		result = true;
	}

	return result;
}

//Run the Lua Interface Class
void LuaFile::Run()
{
	//4. Read the variables
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
void LuaFile::Drop()
{
	if (theLuaState)
	{
		//Close lua state
		lua_close(theLuaState);
		theLuaState = nullptr;
		cout << "lua file Closed";
	}
}

lua_State* LuaFile::GetLuaState(PassKey<LuaManager> _key)
{
	return theLuaState;
}

int LuaFile::GetIntValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return lua_tointeger(theLuaState, -1);
}

//Get a float value through the Lua Interface Class
float LuaFile::GetFloatValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return (float)lua_tonumber(theLuaState, -1);
}

//Get a char value through the Lua Interface Class
char LuaFile::GetCharValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);

	size_t len;
	const char* cstr = lua_tolstring(theLuaState, -1, &len);
	// If the string is not empty, then return the first char
	if (len > 0)
		return cstr[0];
	//else return a default value of <SPACE>
	else
		return ' ';
}

Vector3 LuaFile::GetVector3Values(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	lua_rawgeti(theLuaState, -1, 1);
	int x = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	lua_rawgeti(theLuaState, -1, 2);
	int y = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	lua_rawgeti(theLuaState, -1, 3);
	int z = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	return Vector3(x, y, z);
}

float LuaFile::GetDistanceSquareValue(const char* varName,
	Vector3 source,
	Vector3 destination)
{
	lua_getglobal(theLuaState, varName);
	lua_pushnumber(theLuaState, source.x);
	lua_pushnumber(theLuaState, source.y);
	lua_pushnumber(theLuaState, source.z);
	lua_pushnumber(theLuaState, destination.x);
	lua_pushnumber(theLuaState, destination.y);
	lua_pushnumber(theLuaState, destination.z);
	lua_call(theLuaState, 6, 1);
	float distanceSquare = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return distanceSquare;
}

// Save an integer value through the Lua Interface Class
void LuaFile::SaveIntValue(const char* varName, const int value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	//sprintf(outputString, "%s = %d\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
	//cout << "....................." << endl;
}

//Save a float value through the Lua Interface Class
void LuaFile::SaveFloatValue(const char* varName, const float value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	//sprintf(outputString, "%s = %6.4f\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
}

void LuaFile::GetVariableValues(const char* varName, int &a, int &b, int &c, int &d)
{
	lua_getglobal(theLuaState, varName);
	lua_pushnumber(theLuaState, 1000);
	lua_pushnumber(theLuaState, 2000);
	lua_pushnumber(theLuaState, 3000);
	lua_pushnumber(theLuaState, 4000);
	lua_call(theLuaState, 4, 4);
	a = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	b = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	c = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	d = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
}

float LuaFile::GetField(const char *key)
{
	int result;
	lua_pushstring(theLuaState, key);
	lua_gettable(theLuaState, -2);
	result = (int)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1); /*remove number*/
	return result;
}

Vector3 LuaFile::GetWayPoint(int waypointNumber)
{
	string waypoint = "Point" + std::to_string(waypointNumber);
	lua_getglobal(theLuaState, waypoint.c_str());

	return Vector3(GetField("x"), GetField("y"), GetField("z"));
}

Vector3 LuaFile::GenerateWayPoints(float center, float range)
{
	lua_getglobal(theLuaState, "GenerateWayPoint");
	if (lua_isfunction(theLuaState, lua_gettop(theLuaState)))
	{
		lua_pushnumber(theLuaState, center);
		lua_pushnumber(theLuaState, range);
		lua_call(theLuaState, 2, 3);

		float x, y, z;
		if (lua_isnumber(theLuaState, lua_gettop(theLuaState)))
		{
			x = lua_tonumber(theLuaState, -1);
			lua_pop(theLuaState, 1);
		}
		if (lua_isnumber(theLuaState, lua_gettop(theLuaState)))
		{
			y = lua_tonumber(theLuaState, -1);
			lua_pop(theLuaState, 1);
		}
		if (lua_isnumber(theLuaState, lua_gettop(theLuaState)))
		{
			z = lua_tonumber(theLuaState, -1);
			lua_pop(theLuaState, 1);
		}
		return Vector3(x, y, z);
	}

	return Vector3(0, 0, 0);
}

//float LuaFile::GetField(const char *key)
//{
//	int result = false;
//
//	//check if the variables in the lua stack belongs to a table
//	if (!lua_istable(theLuaState, -1))
//		error("error100");
//
//	lua_pushstring(theLuaState, key);
//	lua_gettable(theLuaState, -2);
//	if (!lua_isnumber(theLuaState, -1))
//		error("error101");
//	result = (int)lua_tonumber(theLuaState, -1);
//	lua_pop(theLuaState, -1);
//	return result;
//}

//void LuaFile::error(const char *errorCode)
//{
//	if (theErrorState == NULL)
//		return;
//
//	lua_getglobal(theErrorState, errorCode);
//	const char *errorMsg = lua_tostring(theErrorState, -1);
//	if (errorMsg != NULL)
//		cout << errorMsg << endl;
//	else
//		cout << errorCode << " is not valid.\n*** Please contact the developer ***" << endl;
//
//}
