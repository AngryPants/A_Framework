#include "SpriteRenderer.h"
#include "../../GameObject/GameObject.h"

//Constructor(s) & Destructor
SpriteRenderer::SpriteRenderer(GameObject& gameObject) : Component("Sprite Renderer", gameObject) {
	mesh = nullptr;
	lightEnabled = false;
}

SpriteRenderer::~SpriteRenderer() {
}

//Interface
void SpriteRenderer::Update(double deltaTime) {
	if (mesh != nullptr) {
		map<string, Animation>::iterator mapIter = animations.find(currentAnimation);
		if (mapIter != animations.end()) {
			mesh->animation = &mapIter->second;
		}
		mesh->Update(deltaTime);
		mesh->animation = nullptr;
	}
}

void SpriteRenderer::AddAnimation(const string& animName, Animation animation) {
	map<string, Animation>::iterator mapIter = animations.find(animName);
	if (mapIter != animations.end()) {
		animations.erase(mapIter);
	}
	animations.insert(std::pair<string, Animation>(animName, animation));
}

void SpriteRenderer::SetAnimation(const string& animName) {
	map<string, Animation>::iterator mapIter = animations.find(animName);
	if (mapIter != animations.end()) {
		mapIter->second.Reset(true);
		currentAnimation = animName;
	} else {
		currentAnimation.clear();
	}
}

void SpriteRenderer::RemoveAnimation(const string& name) {
	map<string, Animation>::iterator mapIter = animations.find(name);
	if (mapIter != animations.end()) {
		animations.erase(mapIter);
		if (name == currentAnimation) {
			currentAnimation.clear();
		}
	}
}

void SpriteRenderer::RemoveAllAnimation() {
	animations.clear();
	currentAnimation.clear();
}

const string& SpriteRenderer::GetCurrentAnimation() const {
	return this->currentAnimation;
}

Animation* SpriteRenderer::GetAnimation(const string& animName) {
	map<string, Animation>::iterator mapIter = animations.find(animName);
	if (mapIter != animations.end()) {
		return &mapIter->second;
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
	if (mesh != nullptr) {
		map<string, Animation>::iterator mapIter = animations.find(currentAnimation);
		if (mapIter != animations.end()) {
			mesh->animation = &mapIter->second;
		}
	}
	return mesh;
}

void SpriteRenderer::ClearAll() {
	mesh = nullptr;
	animations.clear();
	currentAnimation.clear();
}