#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include "../Mesh/Mesh.h"
#include <vector>
#include "Animation.h"

class SpriteAnimation : public Mesh {

public:	
	Animation* animation;

	//Constructor(s) & Destructor
	SpriteAnimation(const string& meshName, unsigned int row, unsigned int col);
	virtual ~SpriteAnimation();

	//Function(s)
	void Update(double deltaTime);
	void Reset(bool animActive);
	virtual void Render();

};

#endif