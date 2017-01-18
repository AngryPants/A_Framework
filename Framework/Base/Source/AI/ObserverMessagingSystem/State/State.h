#ifndef STATE_H
#define STATE_H

#include <string>
using std::string;

class Message;

class State {

protected:
	//Constructor(s)
	State(const string& name) : name(name) {}

public:
	//Variable(s)
	const string name;

	//Destructor	
	virtual ~State() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) = 0;
	virtual void ReceiveMessage(const Message* messagePtr) = 0; // do we need this?

};

#endif