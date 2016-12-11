#include "MeshHolder.h"
#include "MeshRenderer.h"
#include "LODMeshHolder.h"

//Constructor(s) & Destructor
MeshHolder::MeshHolder(GameObject& _gameObject) : Component("Mesh Holder", _gameObject) {
	mesh = nullptr;

	if (GetGameObject().HasComponent<LODMeshHolder>()) {
		GetGameObject().RemoveComponent<LODMeshHolder>();
	}
	if (!GetGameObject().HasComponent<MeshRenderer>()) {
		GetGameObject().AddComponent<MeshRenderer>();
	}
}

MeshHolder::~MeshHolder() {
}

//Mesh
bool MeshHolder::SetMesh(Mesh* _mesh) {
	this->mesh = _mesh;
	return true;
}
bool MeshHolder::RemoveMesh() {
	this->mesh = nullptr;
	return true;
}
Mesh* MeshHolder::GetMesh() {
	return this->mesh;
}

//Texture
bool MeshHolder::SetTextures(const TextureList& _textureList) {
	this->textureList = _textureList;
	return true;
}
bool MeshHolder::RemoveTextureList() {
	this->textureList.Reset();
	return true;
}
TextureList& MeshHolder::GetTextureList() {
	return this->textureList;
}