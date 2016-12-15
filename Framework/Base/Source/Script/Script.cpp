#include "Script.h"
#include "../GameObject/GameObject.h"

//Constructor(s) & Destructor
Script::Script(GameObject& owner) {
	gameObject = &owner;
}

Script::~Script() {
}

//Interface Function(s)
GameObject& Script::GetGameObject() {
	return *gameObject;
}

//Collision Callbacks
void Script::OnCollisionEnter(const CollisionInfo& _info) {
}

void Script::OnCollisionStay(const CollisionInfo& _info) {
}

void Script::OnCollisionExit(const CollisionInfo& _info) {
}

//Trigger Callbacks
void Script::OnTriggerEnter(const Collider& _collider) {
}

void Script::OnTriggerStay(const Collider& _collider) {
}

void Script::OnTriggerExit(const Collider& _collider) {
}