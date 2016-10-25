#ifndef COMPONENT_H
#define COMPONENT_H

#include "Typedef.h"
#include <string>

using std::string;

class Component {

private:
	//Variable(s)
	EntityIndex ownerIndex;

public:
	//Variable(s)
	const string name;

	//Destructor
	virtual ~Component() {}

	//Interface Function(s)
	EntityIndex GetOwnerIndex() {
		return ownerIndex;
	}

protected:
	//Constructor(s)
	Component(const string& name, EntityIndex ownerIndex) : name(name) {
		this->ownerIndex = ownerIndex;
	}

};

#endif