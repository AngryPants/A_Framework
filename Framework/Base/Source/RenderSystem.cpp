#include "RenderSystem.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"

//Include Components
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

//Constructor(s) & Destructor
RenderSystem::RenderSystem() {
}

RenderSystem::~RenderSystem() {
}

//Interface Function(s)
void RenderSystem::Render(const string& space) {
	
	//Cameras
	Camera* camPtr = nullptr;
	set<Component*>& cameras = ComponentManager::GetInstance().GetComponents<Camera>(space);
	for (set<Component*>::iterator setIter = cameras.begin(); setIter != cameras.end(); ++setIter) {
		Camera* camPtr2 = static_cast<Camera*>(*setIter);
		if (camPtr2->isActive == false) {
			continue;
		}
		if (camPtr == nullptr) {
			if (camPtr2->GetGameObject().HasComponent<Transform>()) {
				camPtr = camPtr2;
			}
		} else {
			if (camPtr2->depth > camPtr->depth) {
				camPtr = camPtr2;
			}
		}		
	}

	//Ensure that we have a camera to even render,
	if (camPtr == nullptr) {
		return;
	}
	GraphicsManager::GetInstance().SetToCameraView(*camPtr, camPtr->GetGameObject().GetComponent<Transform>());	

	//Lights
	set<Component*>& lights = ComponentManager::GetInstance().GetComponents<Light>(space);
	unsigned int lightIndex = 0;
	for (set<Component*>::iterator setIter = lights.begin(); setIter != lights.end() && lightIndex < RenderHelper::GetInstance().GetNumLights(); ++setIter) {
		Light* lightPtr = static_cast<Light*>(*setIter);
		if (lightPtr->isActive && lightPtr->GetGameObject().HasComponent<Transform>()) {
			RenderHelper::GetInstance().UpdateLight(*lightPtr, lightPtr->GetGameObject().GetComponent<Transform>(), lightIndex++);
		}
	}
	while (lightIndex < RenderHelper::GetInstance().GetNumLights()) {
		RenderHelper::GetInstance().TurnOffLight(lightIndex++);
	}

	//Now Render the meshes
	set<Component*>& meshRenderers = ComponentManager::GetInstance().GetComponents<MeshRenderer>(space);
	//Loop through them.
	for (set<Component*>::iterator setIter = meshRenderers.begin(); setIter != meshRenderers.end(); ++setIter) {
		MeshRenderer& meshRenderer = *(static_cast<MeshRenderer*>(*setIter));
		//Check if the GameObject has the required components.
		if (meshRenderer.GetGameObject().HasComponent<Transform>() == false) {
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

void RenderSystem::RenderUI(const string& space) {
}