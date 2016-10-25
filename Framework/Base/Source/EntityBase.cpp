#include "EntityBase.h"

EntityBase::EntityBase(const std::string& name) : name(name), isDestroyed(false) {	
}

EntityBase::~EntityBase() {
	isDestroyed = true;
}

void EntityBase::Destroy() {
	this->isDestroyed = true;
}

bool EntityBase::IsDestroyed() {
	return this->isDestroyed;
}
void EntityBase::Update(const double& deltaTime) {
}

void EntityBase::Render() {
}

void EntityBase::RenderUI() {
}