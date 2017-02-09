#ifndef LUA_INTERFACE_H
#define LUA_INTERFACE_H

#include "../Lua/lua.hpp"
#include "../Source/PassKey.h"
#include "../Source/Vector3.h"
#include <map>
#include <string>
using std::string;
using std::map;

//class LuaManager;

class LuaFile
{
private:
	lua_State* theLuaState;
public:
	LuaFile(/*PassKey<LuaManager> _key*/);
	~LuaFile();

	//Initialization of the Lua Interface Class
	bool Init(string filePath);

	//Run the Lua Interface Class , can be inherited to do different things
	virtual void Run();

	void CallFunction(const std::string& funcName, ...);

	void PushVariable(/*different types Can use template here*/);
	void CallFunction();

	//Drop the LuaFile Class
	void Drop();



	////Custom functions 
	//int GetIntValue(const char* varName);

	//float GetFloatValue( const char* varName);

	////get char value through lua interface class
	//char GetCharValue(const char* varName);

	////get vector3 values through the lua interface class
	//Vector3 GetVector3Values(const char* varName);

	////get distance square value through the Lua Interface Class
	//float GetDistanceSquareValue(const char* varName, Vector3 source, Vector3 destination);

	//void GetVariableValues(const char* varName, int &a, int &b, int &c, int &d);

	//void SaveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);

	//void SaveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);

	//float GetField(const char *key);

	//void error(const char *errorCode);

};

#endif