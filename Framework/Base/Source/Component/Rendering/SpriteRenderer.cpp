#include "SpriteRenderer.h"
#include "../../GameObject/GameObject.h"

//Constructor(s) & Destructor
SpriteRenderer::SpriteRenderer(GameObject& gameObject) : Component("Sprite Renderer", gameObject) {
	mesh = nullptr;
	lightEnabled = false;
	currentAnimation = nullptr;
}

SpriteRenderer::~SpriteRenderer() {
}

//Interface
void SpriteRenderer::Update(double deltaTime) {
	if (mesh != nullptr) {
		mesh->animation = currentAnimation;
		mesh->Update(deltaTime);
		mesh->animation = nullptr;
	}
}

void SpriteRenderer::AddAnimation(const string& animName, Animation& animation) {
	map<string, Animation*>::iterator mapIter = animations.find(animName);
	if (mapIter != animations.end()) {
		delete mapIter->second;
		mapIter->second = &animation;
	} else {
		animations.insert(std::pair<string, Animation*>(animName, &animation));
	}	
}

void SpriteRenderer::SetAnimation(const string& animName, bool reset, bool animActive) {
	currentAnimation = nullptr;
	map<string, Animation*>::iterator mapIter = animations.find(animName);
	if (mapIter != animations.end()) {
		if (reset) {
			mapIter->second->Reset(animActive);
		}		
		currentAnimation = mapIter->second;
	}
}

void SpriteRenderer::RemoveAnimation(const string& name) {
	map<string, Animation*>::iterator mapIter = animations.find(name);
	if (mapIter != animations.end()) {
		if (currentAnimation == mapIter->second) {
			currentAnimation = nullptr;
		}
		delete mapIter->second;
		animations.erase(mapIter);
	}
}

void SpriteRenderer::RemoveAllAnimation() {
	currentAnimation = nullptr;
	for (map<string, Animation*>::iterator mapIter = animations.begin(); mapIter != animations.end(); ++mapIter) {
		delete mapIter->second;
	}
	animations.clear();
}

Animation* SpriteRenderer::GetCurrentAnimation() {
	return this->currentAnimation;
}

Animation* SpriteRenderer::GetAnimation(const string& animName) {
	map<string, Animation*>::iterator mapIter = animations.find(animName);
	if (mapIter != animations.end()) {
		return mapIter->second;
	}
	return nullptr;
}

void SpriteRenderer::SetSpriteAnimation(SpriteAnimation& mesh) {
	this->mesh = &mesh;
}

void SpriteRenderer::RemoveSpriteAnimation() {
	this->mesh = nullptr;
}

SpriteAnimation* SpriteRenderer::GetSpriteAnimation() {
	mesh->animation = currentAnimation;
	return mesh;
}

void SpriteRenderer::ClearAll() {
	RemoveSpriteAnimation();
	RemoveAllAnimation();
}