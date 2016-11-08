#include "RenderSystem.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"

//Include Components
#include "MeshRenderer.h"
#include "Transform.h"

//Constructor(s) & Destructor
RenderSystem::RenderSystem() {
	componentBitset[IDGenerator::GetInstance().GetComponentTypeID<Transform>()] = 1;
	componentBitset[IDGenerator::GetInstance().GetComponentTypeID<MeshRenderer>()] = 1;
}

RenderSystem::~RenderSystem() {
}

//Interface Function(s)
void RenderSystem::Render(const string& space) {

	set<Component*>& transforms = ComponentManager::GetInstance().GetComponents<Transform>(space);
	set<Component*>& meshRenderers = ComponentManager::GetInstance().GetComponents<MeshRenderer>(space);

	for (set<Component*>::iterator setIter = meshRenderers.begin(); setIter != meshRenderers.end(); ++setIter) {
		MeshRenderer& meshRenderer = *(static_cast<MeshRenderer*>(*setIter));
		if ((meshRenderer.GetGameObject().GetComponentBitset() & this->componentBitset) != this->componentBitset) {
			continue;
		}
		Transform& transform = meshRenderer.GetGameObject().GetComponent<Transform>();
		if (!meshRenderer.isActive || !transform.isActive || meshRenderer.mesh == nullptr) {
			continue;
		}

		
		MS& modelStack = GraphicsManager::GetInstance().modelStack;
		modelStack.PushMatrix();
			modelStack.Translate(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
			modelStack.Rotate(transform.GetRotation().y, 0, 1, 0);
			modelStack.Rotate(transform.GetRotation().x, 1, 0, 0);			
			modelStack.Rotate(transform.GetRotation().z, 0, 0, 1);
			modelStack.Scale(transform.GetScale().x, transform.GetScale().y, transform.GetScale().z);
			RenderHelper::GetInstance().RenderMesh(*meshRenderer.mesh, meshRenderer.textureList, meshRenderer.lightEnabled);
		modelStack.PopMatrix();
	}	

}