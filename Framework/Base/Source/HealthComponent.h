#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "Component.h"
#include "MyMath.h"

class HealthComponent : public Component {

private:
	//Variable(s)
	int hp;
	int maxHp;

public:
	//Constructor(s) & Destructor
	HealthComponent(GameObject& gameObject) : Component("Health Component", gameObject) {
		hp = 1;
		maxHp = 10;
	}
	virtual ~HealthComponent() {}

	//Interface Function(s)
	bool IsAlive() {
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
	void SetMaxHealth(const int maxHealth) {
		if (maxHealth > 0) {
			this->maxHp = maxHealth;
		} else {
			this->maxHp = 0;
		}
		if (this->hp > this->maxHp) {
			this->hp = this->maxHp;
		}
	}
	int GetMaxHealth() const {
		return this->maxHp;
	}

};

#endif