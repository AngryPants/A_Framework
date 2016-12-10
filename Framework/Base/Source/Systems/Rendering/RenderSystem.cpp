#include "RenderSystem.h"

//Include Systems
#include "../../Component/ComponentManager.h"
#include "../../GameObject/GameObject.h"
#include "../../Graphics/RenderHelper.h"
#include "../../Graphics/GraphicsManager.h"
#include "../../Application/Application.h"
#include "../../Scene/SceneGraph.h"
#include "../../Scene/SceneNode.h"
#include "../../Systems/SpatialPartition/SPSystem.h"
#include "../../Mesh/MeshBuilder.h"

//Include Components
#include "../../Component/Rendering/MeshRenderer.h"
#include "../../Component/Rendering/MeshHolder.h"
#include "../../Component/Rendering/LODMeshHolder.h"
#include "../../Component/Rendering/SpriteRenderer.h"
#include "../../Component/Rendering/TextRenderer.h"
#include "../../Component/Physics/Transform.h"
#include "../../Component/Rendering/Camera.h"
#include "../../Component/Rendering/Light.h"

//Others
#include <stack>

using std::stack;

//Constructor(s) & Destructor
RenderSystem::RenderSystem() {
	gridMesh = MeshBuilder::GetInstance().GenerateWireframeCube("Grid Mesh", Color(0, 1, 0), 1.0f);
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::RenderRecursion(GameObject& gameObject) {
	//Get our transform.
	Transform& transform = gameObject.GetComponent<Transform>();

	//Get our modelStack from GraphicsManager.
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	modelStack.PushMatrix();
		/*modelStack.Translate(transform.GetLocalPosition().x, transform.GetLocalPosition().y, transform.GetLocalPosition().z);
		modelStack.Rotate(transform.GetLocalRotation().y, 0, 1, 0);
		modelStack.Rotate(transform.GetLocalRotation().x, 1, 0, 0);
		modelStack.Rotate(transform.GetLocalRotation().z, 0, 0, 1);
		modelStack.Scale(transform.GetLocalScale().x, transform.GetLocalScale().y, transform.GetLocalScale().z);*/
		//Check for MeshRenderer.
		if (gameObject.HasComponent<MeshRenderer>()) {
			if (gameObject.HasComponent<MeshHolder>()) {
				modelStack.MultMatrix(transform.GetTransformationMatrix());
				MeshRenderer& meshRenderer = gameObject.GetComponent<MeshRenderer>();
				MeshHolder& meshHolder = gameObject.GetComponent<MeshHolder>();
				RenderHelper::GetInstance().RenderMesh(*meshHolder.mesh, meshHolder.textureList, meshRenderer.lightEnabled);
			} else if (gameObject.HasComponent<LODMeshHolder>()) {
				//Do Stuff
			}
		}
	modelStack.PopMatrix();
		
	//Recursion in the children.
	vector<GameObject*> children;
	gameObject.GetChildren(children);
	for (unsigned int i = 0; i < children.size(); ++i) {
		RenderRecursion(*children[i]);
	}	

	//modelStack.PopMatrix();
}

//Interface Function(s)
void RenderSystem::Update(const string& space, double deltaTime) {
	//Sprite Animation
	set<Component*>& spriteRenderers = ComponentManager::GetInstance().GetComponents<SpriteRenderer>(space);
	for(set<Component*>::iterator setIter = spriteRenderers.begin(); setIter != spriteRenderers.end(); ++setIter) {
		SpriteRenderer& spriteRenderer = *(static_cast<SpriteRenderer*>(*setIter));
		if(!spriteRenderer.isActive) {
			continue;
		}
		spriteRenderer.Update(deltaTime);
	}
}

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
		cout << "No suitable camera. Unable to Render()." << endl;
		return;
	}
	//Camera Debugging
	/*static int debugTimer = 1;
	if (debugTimer < 0.0f) {
		cout << "Camera Position" << camPtr->GetGameObject().GetComponent<Transform>().GetPosition() << endl;
		cout << "Camera Forward: " << camPtr->GetGameObject().GetComponent<Transform>().GetForward() << endl;
		cout << "Camera Up: " << camPtr->GetGameObject().GetComponent<Transform>().GetUp() << endl;
		cout << "Camera Left: " << camPtr->GetGameObject().GetComponent<Transform>().GetLeft() << endl;
		debugTimer = 240;
	}
	debugTimer -= 1;*/

	camPtr->aspectRatio.Set(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
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
	/*set<Component*>& meshRenderers = ComponentManager::GetInstance().GetComponents<MeshRenderer>(space);
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
	}*/

	SceneNode* rootNode = SceneGraph::GetInstance().GetRootNode(space);
	set<SceneNode*>& rootChildren = rootNode->GetChildren();
	for (set<SceneNode*>::iterator nodeIter = rootChildren.begin(); nodeIter != rootChildren.end(); ++nodeIter) {
		SceneNode* nodePtr = *nodeIter;
		RenderRecursion(*nodePtr->GetGameObject());
	}

	//RenderGrid(space);

	//Sprite Animation
	/*set<Component*>& spriteRenderers = ComponentManager::GetInstance().GetComponents<SpriteRenderer>(space);
	for (set<Component*>::iterator setIter = spriteRenderers.begin(); setIter != spriteRenderers.end(); ++setIter) {
		SpriteRenderer& spriteRenderer = *(static_cast<SpriteRenderer*>(*setIter));		
		if (spriteRenderer.GetGameObject().HasComponent<Transform>() == false) {
			continue;
		}
		Transform& transform = spriteRenderer.GetGameObject().GetComponent<Transform>();
		if (!spriteRenderer.isActive || !transform.isActive || spriteRenderer.GetSpriteAnimation() == nullptr) {
			continue;
		}

		MS& modelStack = GraphicsManager::GetInstance().modelStack;
		modelStack.PushMatrix();
			modelStack.Translate(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
			modelStack.Rotate(transform.GetLocalRotation().y, 0, 1, 0);
			modelStack.Rotate(transform.GetLocalRotation().x, 1, 0, 0);
			modelStack.Rotate(transform.GetLocalRotation().z, 0, 0, 1);
			modelStack.Scale(transform.GetLocalScale().x, transform.GetLocalScale().y, transform.GetLocalScale().z);
			RenderHelper::GetInstance().RenderMesh(*spriteRenderer.GetSpriteAnimation(), spriteRenderer.textureList, spriteRenderer.lightEnabled);
		modelStack.PopMatrix();
	}*/

	//Text
	/*set<Component*>& textRenderers = ComponentManager::GetInstance().GetComponents<TextRenderer>(space);
	//Loop through them.
	for (set<Component*>::iterator setIter = textRenderers.begin(); setIter != textRenderers.end(); ++setIter) {
		TextRenderer& textRenderer = *(static_cast<TextRenderer*>(*setIter));
		//Check if the GameObject has the required components.
		if (textRenderer.GetGameObject().HasComponent<Transform>() == false) {
			continue;
		}
		Transform& transform = textRenderer.GetGameObject().GetComponent<Transform>();
		if (!textRenderer.isActive || !transform.isActive || textRenderer.mesh == nullptr) {
			continue;
		}

		MS& modelStack = GraphicsManager::GetInstance().modelStack;
		modelStack.PushMatrix();
			modelStack.Translate(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
			modelStack.Rotate(transform.GetLocalRotation().y, 0, 1, 0);
			modelStack.Rotate(transform.GetLocalRotation().x, 1, 0, 0);
			modelStack.Rotate(transform.GetLocalRotation().z, 0, 0, 1);
			modelStack.Scale(transform.GetLocalScale().x, transform.GetLocalScale().y, transform.GetLocalScale().z);
			RenderHelper::GetInstance().RenderText(*textRenderer.mesh, textRenderer.textureList, textRenderer.text, textRenderer.textColor);
		modelStack.PopMatrix();
	}*/
}

void RenderSystem::RenderUI(const string& space) {
}

void RenderSystem::RenderGrid(const string& space) {
	SpatialPartition* sp = SpatialPartitionSystem::GetInstance().GetSpatialPartition(space);
	if (sp == nullptr) {
		return;
	}
	
	//Get our modelStack from GraphicsManager.
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	for (int x = 0; x < sp->GetxNumOfGrid(); ++x) {
		for (int y = 0; y < sp->GetyNumOfGrid(); ++y) {
			for (int z = 0; z < sp->GetzNumOfGrid(); ++z) {
				modelStack.PushMatrix();
					Grid& grid = sp->GetGrid(x, y, z);
					Vector3 position = grid.GetPosition();
					modelStack.Translate(position.x, position.y, position.z);
					//cout << "Grid Size X: " << sp->GetxGridSize() << endl;
					//cout << "Grid Size Y: " << sp->GetyGridSize() << endl;
					//cout << "Grid Size Z: " << sp->GetzGridSize() << endl;
					modelStack.Scale(sp->GetxGridSize(), sp->GetyGridSize(), sp->GetzGridSize());
					RenderHelper::GetInstance().RenderMesh(*gridMesh);
				modelStack.PopMatrix();
			}
		}
	}	
}