#ifndef SPEHRE_COLLIDER_H
#define SPEHRE_COLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider {

private:
	//Variable(s)
	float radius;

public:
	//Constructor(s) & Destructor
	SphereCollider(GameObject& gameObject);
	virtual ~SphereCollider();

	//Interface Function(s)
	void SetRadius(const float radius);
	float GetRadius() const;

};

#endif