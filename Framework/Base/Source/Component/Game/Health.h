#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H
#include "../Component.h"

class HealthComponent : public Component
{
private:
	// variables
	int healthpoints;
	int maxHealth;
public:
	// constructor
	HealthComponent(GameObject& gameObject, PassKey<ComponentManager> _key)
		: Component("Health Component", gameObject, _key)
		, healthpoints(100)
		, maxHealth(100)
	{
	} 
	virtual ~HealthComponent(){}

	void SetMaxHealth(int maxHP)
	{
		if (maxHP > 0)
		{
			maxHealth = maxHP;
			if (healthpoints > maxHealth)
				healthpoints = maxHealth;
		}
	}

	void SetHealth(int health)
	{
		if (health > 0 && health < maxHealth)
			healthpoints = health;
	}

	void TakeDamage(int damage)
	{
		healthpoints -= damage;
	}

	int GetCurrentHealth()
	{
		return healthpoints;
	}

	int GetMaxHealth()
	{
		return maxHealth;
	}
};


#endif