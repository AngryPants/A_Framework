#include "MeshRenderer.h"
#include "../../GameObject/GameObject.h"
#include "MeshHolder.h"
#include "LODMeshHolder.h"
#include "../ComponentManager.h"

//Constructor(s) & Destructor
MeshRenderer::MeshRenderer(GameObject& _gameObject, PassKey<ComponentManager> _key) : Component("Mesh Renderer", _gameObject, _key) {
	lightEnabled = true;
	if (!GetGameObject().HasComponent<MeshHolder>() && !GetGameObject().HasComponent<LODMeshHolder>()) {
		GetGameObject().AddComponent<MeshHolder>();
	}
}

MeshRenderer::~MeshRenderer() {
}