#include "MeshRenderer.h"
#include "../../GameObject/GameObject.h"
#include "MeshHolder.h"
#include "LODMeshHolder.h"

//Constructor(s) & Destructor
MeshRenderer::MeshRenderer(GameObject& gameObject) : Component("Mesh Renderer", gameObject) {
	lightEnabled = true;
	if (!GetGameObject().HasComponent<MeshHolder>() && !GetGameObject().HasComponent<LODMeshHolder>()) {
		GetGameObject().AddComponent<MeshHolder>();
	}
}

MeshRenderer::~MeshRenderer() {
}