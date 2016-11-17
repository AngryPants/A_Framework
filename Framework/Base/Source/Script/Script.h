#ifndef SCRIPT_H
#define SCRIPT_H

class GameObject;

class Script {

private:
	//Variable(s)
	GameObject* gameObject; //The GameObject that owns this Script.

public:
	//Constructor(s) & Destructor
	Script(GameObject& gameObject);
	virtual ~Script();

	//Interface Function(s)
	GameObject& GetGameObject();
	virtual void Update(double deltaTime) = 0;	

};

#endif