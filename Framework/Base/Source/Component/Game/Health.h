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
	HealthComponent(GameObject& gameObject,int health = 0) 
		: Component("health",gameObject)
		, healthpoints(health)
		, maxHealth(health)
	{
	} 
	virtual ~HealthComponent(){}

	void SetMaxHealth(int maxHP)
	{
		if (maxHP > 0)
			maxHealth = maxHP;
	}

	void SetHealth(int health)
	{
		if (health > 0 && health < maxHealth)
			healthpoints = health;
		else if (maxHealth == 0)
			maxHealth = healthpoints = health;
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