#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "../Component.h"
#include "../../Animation/SpriteAnimation.h"
#include "../../Animation/Animation.h"
#include "../../Texture/TextureList.h"
#include <map>

using std::map;

class SpriteRenderer : public Component {
	
private:
	//Variable(s)
	SpriteAnimation* mesh;
	map<string, Animation> animations;
	string currentAnimation;

public:
	//Variable(s)	
	TextureList textureList;
	bool lightEnabled;

	//Constructor(s) & Destructor
	SpriteRenderer(GameObject& gameObject);
	virtual ~SpriteRenderer();

	//Interface
	void Update(double deltaTime);
	void AddAnimation(const string& animName, Animation animation);
	void SetAnimation(const string& animName);
	void RemoveAnimation(const string& animName);
	void RemoveAllAnimation();
	const string& GetCurrentAnimation() const;
	Animation* GetAnimation(const string& animName);
	void SetSpriteAnimation(SpriteAnimation& mesh);
	void RemoveSpriteAnimation();
	SpriteAnimation* GetSpriteAnimation();
	void ClearAll();

};

#endif