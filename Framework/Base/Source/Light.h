#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h"
#include "Transform.h"

struct Light {

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
	bool on;
	Transform transform;
	//Position position;
	//Vector3 spotDirection;

	//Constructor(s) & Destructor
	Light() {
		type = LIGHT_TYPE::LIGHT_POINT;
		color.Set(1, 1, 1);
		power = 1.0f;
		kC = 1.0f;
		kL = 0.1f;
		kQ = 0.1f;
		cosInner = 0.8f;
		cosCutoff = 1.0f;
		exponent = 0.1f;
		on = true;
	}
	~Light() {}

};

#endif