#include "Error.h"
#include <exception>
#include <iostream>

using std::exception;
using std::cout;
using std::endl;

namespace Error {

	void ThrowException(const char* errorMessage) {
		PrintToConsole(errorMessage);
		throw exception(errorMessage);
	}
	void ThrowException(const std::string errorMessage) {
		ThrowException(errorMessage.c_str());
	}

	void PrintToConsole(const char* errorMessage) {
		cout << errorMessage << endl;
	}
	void PrintToConsole(const string& errorMessage) {
		PrintToConsole(errorMessage.c_str());
	}

}