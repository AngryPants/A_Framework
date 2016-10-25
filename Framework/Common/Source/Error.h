#ifndef ERROR_H
#define ERROR_H

#include <string>

using std::string;

namespace Error {

	void ThrowException(const char* errorMessage);
	void ThrowException(const std::string& errorMessage);
	void PrintToConsole(const char* errorMessage);
	void PrintToConsole(const std::string& errorMessage);

}

#endif