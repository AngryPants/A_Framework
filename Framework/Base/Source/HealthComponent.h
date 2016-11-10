#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "Component.h"
#include "MyMath.h"

class HealthComponent : public Component {

private:
	//Variable(s)
	int hp;

public:
	//Constructor(s) & Destructor
	HealthComponent(GameObject& gameObject) : Component("Health Component", gameObject) {
		hp = 1;
	}
	virtual ~HealthComponent();

	//Interface Function(s)
	bool isAlive() {
		return hp > 0;
	}

	void TakeDamage(const int damage) {
		if (damage > 0) {
			hp = Math::Max(0, hp - damage);
		}		
	}
	void Heal(const int health) {
		if (health > 0) {
			hp += health;
		}		
	}
	void SetHealth(const int health) {
		if (health < 0) {
			this->hp = 0;
		} else {
			this->hp = health;
		}
	}
	int GetHealth() const {
		return hp;
	}

};

#endif