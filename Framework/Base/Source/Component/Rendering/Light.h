#ifndef LIGHT_H
#define LIGHT_H

#include "../Component.h"
#include "../../Mesh/Vertex.h"

class Light : public Component {

public:
	//Enum(s)
	enum class LIGHT_TYPE {
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};

	//Variable(s)
	LIGHT_TYPE type;
	Color color;
	float power;
	float kC;
	float kL;
	float kQ;	
	float cosCutoff;
	float cosInner;
	float exponent;
	
	//Constructor(s) & Destructor
	Light(GameObject& _gameObject, PassKey<ComponentManager> _key);
	virtual ~Light();
	
};

#endif