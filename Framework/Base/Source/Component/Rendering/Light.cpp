#include "Light.h"
#include "../../Component/ComponentManager.h"
#include "../../GameObject/GameObject.h"

//Constructor(s) & Destructor
Light::Light(GameObject& _gameObject, PassKey<ComponentManager> _key) : Component("Light", _gameObject, _key) {
	type = LIGHT_TYPE::LIGHT_POINT;
	color.Set(1, 1, 1);
	power = 1.0f;
	kC = 1.0f;
	kL = 0.1f;
	kQ = 0.1f;
	cosInner = 0.8f;
	cosCutoff = 1.0f;
	exponent = 0.1f;
}

Light::~Light() {
}