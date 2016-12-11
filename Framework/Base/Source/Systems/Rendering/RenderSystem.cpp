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

//Others
#include <stack>

using std::stack;

//Constructor(s) & Destructor
RenderSystem::RenderSystem() {
	gridMesh = MeshBuilder::GetInstance().GenerateWireframeCube("Grid Mesh", Color(1, 0, 0), 1.0f);
	gridMeshEmpty = MeshBuilder::GetInstance().GenerateWireframeCube("Grid Mesh Empty", Color(0, 1, 0), 1.0f);
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::RenderRecursion(GameObject& gameObject) {
	//Get our transform.
	Transform& transform = gameObject.GetComponent<Transform>();

	//Get our modelStack from GraphicsManager.
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	modelStack.PushMatrix();
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
			camPtr = camPtr2;
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
	
	/*SceneNode* rootNode = SceneGraph::GetInstance().GetRootNode(space);
	set<SceneNode*>& rootChildren = rootNode->GetChildren();
	for (set<SceneNode*>::iterator nodeIter = rootChildren.begin(); nodeIter != rootChildren.end(); ++nodeIter) {
		SceneNode* nodePtr = *nodeIter;
		RenderRecursion(*nodePtr->GetGameObject());
	}*/

	RenderGridBoundaries(space);

	Transform& camTransform = camPtr->GetGameObject().GetComponent<Transform>();
	SpatialPartition* sp = SpatialPartitionSystem::GetInstance().GetSpatialPartition(space);
	for (int x = 0; x < sp->GetxNumOfGrid(); ++x) {
		for (int y = 0; y < sp->GetyNumOfGrid(); ++y) {
			for (int z = 0; z < sp->GetzNumOfGrid(); ++z) {
				Grid& grid = sp->GetGrid(x, y, z);
				if (0 == grid.GetNumObjects()) {
					continue;
				}
				if (false == sp->IsVisible(camTransform.GetPosition(), camTransform.GetForward(), x, y, z)) {					
					continue;
				}
				float distanceToGrid = sp->CalculateDistance(camTransform.GetPosition(), x, y, z);
				if (distanceToGrid > camPtr->GetFarClippingPlane()) {
					continue;
				}
				LODMeshHolder::DETAIL_LEVEL detailLevel = LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS;
				float distanceRatio = distanceToGrid / camPtr->GetFarClippingPlane();
				if (distanceRatio > 0.3f && distanceRatio < 0.7f) {
					detailLevel = LODMeshHolder::DETAIL_LEVEL::MID_DETAILS;
				} else if (distanceRatio > 0.7f && distanceRatio < 1.3f) {
					detailLevel = LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS;
				}
				RenderGrid(grid, detailLevel);
			}
		}
	}
	RenderGrid(sp->GetExtraGrid(), LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS);
}

void RenderSystem::RenderGrid(Grid& grid, LODMeshHolder::DETAIL_LEVEL detailLevel) {
	vector<GameObjectID> goList = grid.GetListOfObjects();
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	for (vector<GameObjectID>::iterator vecIter = goList.begin(); vecIter != goList.end(); ++vecIter) {
		Mesh* mesh = nullptr;
		TextureList* textureList = nullptr;
		GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(*vecIter);
		if (go == nullptr) {
			continue;
		} else if (go->HasComponent<MeshRenderer>() == false) {
			continue;
		}

		MeshRenderer& meshRenderer = go->GetComponent<MeshRenderer>();
		if (go->HasComponent<MeshHolder>()) {
			mesh = go->GetComponent<MeshHolder>().GetMesh();
			textureList = &go->GetComponent<MeshHolder>().GetTextureList();
		} else if (go->HasComponent<LODMeshHolder>()) {
			mesh = go->GetComponent<LODMeshHolder>().GetLODMesh(detailLevel);
			textureList = &go->GetComponent<LODMeshHolder>().GetTextureList(detailLevel);
		}
		if (mesh == nullptr) {
			continue;
		}
		modelStack.PushMatrix();
			modelStack.MultMatrix(go->GetComponent<Transform>().GetTransformationMatrix());
			RenderHelper::GetInstance().RenderMesh(*mesh, *textureList, meshRenderer.lightEnabled);
		modelStack.PopMatrix();
	}
}

void RenderSystem::RenderUI(const string& space) {
}

void RenderSystem::RenderGridBoundaries(const string& _space) {
	SpatialPartition* sp = SpatialPartitionSystem::GetInstance().GetSpatialPartition(_space);
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
					modelStack.Scale(sp->GetxGridSize(), sp->GetyGridSize(), sp->GetzGridSize());
					if (grid.GetNumObjects() != 0) {
						RenderHelper::GetInstance().RenderMesh(*gridMesh);
					} else {
						RenderHelper::GetInstance().RenderMesh(*gridMeshEmpty);
					}					
				modelStack.PopMatrix();
			}
		}
	}	
}