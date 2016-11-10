#ifndef LIVES_COMPONENT_H
#define LIVES_COMPONENT_H

#include "Component.h"

class LivesComponent : public Component {

private:
	int lives;

public:
	//Constructor(s) & Destructor
	LivesComponent(GameObject& gameObject) : Component("Lives Component", GameObject) {
		lives = 3;
	}
	virtual ~LivesComponent() {}

	//Interface Function(s)
	void SetLives(const int lives) {
		if (lives < 0) {
			this->lives = 0;
		} else {
			this->lives = lives;
		}
	}
	int GetLives() const {
		return this->lives;
	}
	void LoseLives(const int lives) {
		if (lives > 0) {
			this->lives -= lives;
		}
	}
	void GainLives(const int lives) {
		if (lives > 0) {
			this->lives += lives;
		}
	}

};

#endif