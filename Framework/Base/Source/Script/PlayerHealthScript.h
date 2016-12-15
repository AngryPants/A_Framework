#ifndef PLAYER_HEALTH_SCRIPT_H
#define PLAYER_HEALTH_SCRIPT_H

#include "../GameObject/GameObject.h"
#include "HealthScript.h"

class PlayerHealthScript : public HealthScript {
public:
	//Constructor(s) & Destructor
	PlayerHealthScript(GameObject& gameObject) : HealthScript(gameObject)
	{
	}
	virtual ~PlayerHealthScript() {}

	//Interface Function(s)
	virtual void ZeroHp() 
	{ 
		vector<GameObject*> children;
		GetGameObject().GetChildren(children);
		for (vector<GameObject*>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			if ((*iter)->name == "Player Camera")
			{
				(*iter)->DetachParent();
				vector<GameObject*> cameraChildren;
				(*iter)->GetChildren(cameraChildren);

				for (vector<GameObject*>::iterator iter2 = cameraChildren.begin(); iter2 != cameraChildren.end(); ++iter2)
				{
					(*iter2)->DetachParent();
				}
				break;
			}


		}
		GetGameObject().Destroy();
	}
};

#endif