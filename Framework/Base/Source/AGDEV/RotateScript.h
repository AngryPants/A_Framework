#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Transform.h"

class RotateScript : public Script {

public:
	RotateScript(GameObject& gameObject) : Script(gameObject) {}
	virtual ~RotateScript() {}

	virtual void Update(double deltaTime) {
		GetGameObject().GetComponent<Transform>().Rotate(50 * deltaTime,50 * deltaTime, 0);
	}

};