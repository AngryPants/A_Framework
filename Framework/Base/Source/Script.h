#ifndef SCRIPT_H
#define SCRIPT_H

class Script {

public:
	Script() {}
	virtual ~Script() {}

	virtual void Update(const double deltaTime) {}

};

#endif