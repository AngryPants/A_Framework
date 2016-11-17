#ifndef SCRIPT_H
#define SCRIPT_H

class Script {

public:
	//Constructor(s) & Destructor
	Script() {}
	virtual ~Script() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) {}

};

#endif