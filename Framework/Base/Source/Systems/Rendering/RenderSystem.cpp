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
	renderSpatialPartition = false;
	gridMesh = MeshBuilder::GetInstance().GenerateWireframeCube("Grid Mesh", Color(1, 0, 0), 1.0f);
	gridMeshEmpty = MeshBuilder::GetInstance().GenerateWireframeCube("Grid Mesh Empty", Color(0, 1, 0), 1.0f);

	renderColliders = false;
	aabbColliderMesh = MeshBuilder::GetInstance().GenerateWireframeCube("AABB Collider Mesh", Color(0, 0, 0), 1.0f);
	sphereColliderMesh = MeshBuilder::GetInstance().GenerateWireframeSphere("Sphere Collider Mesh", Color(0, 0, 0), 8, 8, 0.5f);
}

RenderSystem::~RenderSystem() {
}

//Interface Function(s)
void RenderSystem::Update(const string& space, double deltaTime) {
	//Sprite Animation
	set<Component*>& spriteRenderers = ComponentManager::GetInstance().GetComponents<SpriteRenderer>(space);
	for(set<Component*>::iterator setIter = spriteRenderers.begin(); setIter != spriteRenderers.end(); ++setIter) {
		SpriteRenderer& spriteRenderer = *(static_cast<SpriteRenderer*>(*setIter));
		if(!spriteRenderer.IsActive()) {
			continue;
		}
		spriteRenderer.Update(deltaTime);
	}
}

void RenderSystem::Render(const string& _space, const Skybox* _skybox) {
	
	GraphicsManager::GetInstance().ClearColor();
	GraphicsManager::GetInstance().ClearDepth();

	//Cameras
	Camera* camPtr = nullptr;
	set<Component*>& cameras = ComponentManager::GetInstance().GetComponents<Camera>(_space);
	for (set<Component*>::iterator setIter = cameras.begin(); setIter != cameras.end(); ++setIter) {
		Camera* camPtr2 = static_cast<Camera*>(*setIter);
		if (camPtr2->IsActive() == false) {
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
	
	//Set to the camera's view.
	Transform& camTransform = camPtr->GetGameObject().GetComponent<Transform>();

	camPtr->aspectRatio.Set(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
	GraphicsManager::GetInstance().SetToCameraView(*camPtr, camPtr->GetGameObject().GetComponent<Transform>());	

	//Render Skybox
	if (_skybox != nullptr) {
		RenderSkybox(camTransform.GetPosition(), _skybox);
	}

	//Lights
	set<Component*>& lights = ComponentManager::GetInstance().GetComponents<Light>(_space);
	unsigned int lightIndex = 0;
	for (set<Component*>::iterator setIter = lights.begin(); setIter != lights.end() && lightIndex < RenderHelper::GetInstance().GetNumLights(); ++setIter) {
		Light* lightPtr = static_cast<Light*>(*setIter);
		if (lightPtr->IsActive() && lightPtr->GetGameObject().HasComponent<Transform>()) {
			RenderHelper::GetInstance().UpdateLight(*lightPtr, lightPtr->GetGameObject().GetComponent<Transform>(), lightIndex++);
		}
	}
	while (lightIndex < RenderHelper::GetInstance().GetNumLights()) {
		RenderHelper::GetInstance().TurnOffLight(lightIndex++);
	}

	if (renderSpatialPartition) {
		RenderGridBoundaries(_space);
	}	
	if (renderColliders) {
		RenderColliders(_space);
	}	

	//Render out grid by grid.	
	SpatialPartition* sp = SpatialPartitionSystem::GetInstance().GetSpatialPartition(_space);
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
				float distanceToGridSquare = sp->CalculateDistanceSquare(camTransform.GetPosition(), x, y, z);
				if (distanceToGridSquare > camPtr->GetFarClippingPlane() * camPtr->GetFarClippingPlane()) {
					continue;
				}

				if (camPtr->IsOrtho()) {
					RenderGrid(grid, LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS);
				} else {
					float maxGridSize = sp->GetxGridSize();
					if (sp->GetyGridSize() > maxGridSize) {
						maxGridSize = sp->GetyGridSize();
					}
					if (sp->GetzGridSize() > maxGridSize) {
						maxGridSize = sp->GetzGridSize();
					}
					float angleEstimate = Math::RadianToDegree(atan2(static_cast<float>(maxGridSize), sqrt(distanceToGridSquare)));
					float angleRatio = angleEstimate / camPtr->GetFOV();
									
					if (angleRatio <= camPtr->lodRange[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].GetUpperBounds()) {
						RenderGrid(grid, LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS);
					}
					if (camPtr->lodRange[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].GetLowerBounds() <= angleRatio && angleRatio <= camPtr->lodRange[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].GetUpperBounds()) {
						RenderGrid(grid, LODMeshHolder::DETAIL_LEVEL::MID_DETAILS);
					}
					if (camPtr->lodRange[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].GetLowerBounds() <= angleRatio) {
						RenderGrid(grid, LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS);
					}					
				}
			}
		}
	}
	RenderGrid(sp->GetExtraGrid(), LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS);

	//Render UI here(should be last to render) current camera
	GraphicsManager::GetInstance().ClearDepth();
	if (camPtr->GetGameObject().HasActiveComponent<TextRenderer>())
	{
		RenderUI(camPtr->GetGameObject().GetComponent<TextRenderer>());
	}

}

//Render out a grid.
void RenderSystem::RenderGrid(Grid& grid, LODMeshHolder::DETAIL_LEVEL detailLevel) {
	vector<GameObjectID> goList = grid.GetListOfObjects();
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	for (vector<GameObjectID>::iterator vecIter = goList.begin(); vecIter != goList.end(); ++vecIter) {
		//Get the GameObject.
		GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(*vecIter);
		if (go == nullptr) {
			cout << "RenderSystem has found invalid ID." << endl;
			continue;
		}
		
		//Check for Mesh Renderer.
		if (!go->HasComponent<MeshRenderer>() || !go->GetComponent<MeshRenderer>().IsActive()) {
			continue;
		}

		//Initialise the mesh & textures we need to render.
		Mesh* mesh = nullptr;
		TextureList* textureList = nullptr;

		//Check for type of MeshHolder.
		if (go->HasComponent<MeshHolder>() && go->GetComponent<MeshHolder>().IsActive()) {
			mesh = go->GetComponent<MeshHolder>().GetMesh();
			textureList = &go->GetComponent<MeshHolder>().GetTextureList();
		} else if (go->HasComponent<LODMeshHolder>() && go->GetComponent<LODMeshHolder>().IsActive()) {
			mesh = go->GetComponent<LODMeshHolder>().GetLODMesh(detailLevel);
			textureList = &go->GetComponent<LODMeshHolder>().GetTextureList(detailLevel);
		}

		//Check if we have anything to render.
		if (mesh != nullptr && textureList != nullptr) {
			modelStack.PushMatrix();
				modelStack.MultMatrix(go->GetComponent<Transform>().GetTransformationMatrix());
				RenderHelper::GetInstance().RenderMesh(*mesh, *textureList, go->GetComponent<MeshRenderer>().lightEnabled);
			modelStack.PopMatrix();
		}		
	}
}

void RenderSystem::RenderUI(TextRenderer& textRenderer) {

	if (textRenderer.mesh == nullptr || !textRenderer.IsActive())
		return;

	GraphicsManager::GetInstance().SetToUI();
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	modelStack.PushMatrix();
	modelStack.Translate(textRenderer.position.x, textRenderer.position.y, textRenderer.position.z);
	modelStack.Rotate(textRenderer.rotation, 0, 0, 1);
	modelStack.Scale(textRenderer.scale.x, textRenderer.scale.y, textRenderer.scale.z);
	RenderHelper::GetInstance().RenderText(*textRenderer.mesh, textRenderer.textureList, textRenderer.text, textRenderer.textColor);
	modelStack.PopMatrix();
}

void RenderSystem::RenderSkybox(const Vector3& position, const Skybox* _skybox) {
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	modelStack.PushMatrix();	
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.PushMatrix();
			modelStack.Translate(0, 0.49f, 0);
			modelStack.Rotate(90, 1, 0, 0);
			RenderHelper::GetInstance().RenderMesh(*_skybox->GetMesh(), _skybox->textureLists[Skybox::TEXTURES::SKYBOX_TOP], false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(0, -0.49f, 0);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Rotate(-90, 1, 0, 0);
			RenderHelper::GetInstance().RenderMesh(*_skybox->GetMesh(), _skybox->textureLists[Skybox::TEXTURES::SKYBOX_BOTTOM], false);
		modelStack.PopMatrix();
		for (unsigned int s = 0; s < 4; ++s) {
			modelStack.PushMatrix();		
				modelStack.Rotate(static_cast<float>(s) * 90.0f, 0.0f, 1.0f, 0.0f);
				modelStack.Translate(0, 0, -0.49f);
				RenderHelper::GetInstance().RenderMesh(*_skybox->GetMesh(), _skybox->textureLists[Skybox::TEXTURES::SKYBOX_BACK + s], false);
			modelStack.PopMatrix();
		}
	modelStack.PopMatrix();
	GraphicsManager::GetInstance().ClearDepth();
}

//Render the grid boundaries for debuging purposes.
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

void RenderSystem::RenderColliders(const string& _space) {

	MS& modelStack = GraphicsManager::GetInstance().modelStack;

	set<Component*>& sphereColliders = ComponentManager::GetInstance().GetComponents<ColliderGroup<SphereCollider>>(_space);	
	for (set<Component*>::iterator setIter = sphereColliders.begin(); setIter != sphereColliders.end(); ++setIter) {
		ColliderGroup<SphereCollider>* sphereColliderPtr = static_cast<ColliderGroup<SphereCollider>*>(*setIter);
		for (vector<SphereCollider>::iterator vecIter = sphereColliderPtr->colliders.begin(); vecIter != sphereColliderPtr->colliders.end(); ++vecIter) {
			SphereCollider* collider = &(*vecIter);
			GameObject& go = sphereColliderPtr->GetGameObject();
			Transform& goTransform = go.GetComponent<Transform>();
			modelStack.PushMatrix();
				modelStack.Translate(goTransform.GetPosition().x + collider->centre.x,
									 goTransform.GetPosition().y + collider->centre.y,
									 goTransform.GetPosition().z + collider->centre.z);
				modelStack.Scale(collider->GetRadius() * 2.0f, collider->GetRadius() * 2.0f, collider->GetRadius() * 2.0f);
				RenderHelper::GetInstance().RenderMesh(*sphereColliderMesh);
			modelStack.PopMatrix();
		}
	}

	set<Component*>& aabbColliders = ComponentManager::GetInstance().GetComponents<ColliderGroup<AABBCollider>>(_space);	
	for (set<Component*>::iterator setIter = aabbColliders.begin(); setIter != aabbColliders.end(); ++setIter) {
		ColliderGroup<AABBCollider>* aabbColliderPtr = static_cast<ColliderGroup<AABBCollider>*>(*setIter);
		for (vector<AABBCollider>::iterator vecIter = aabbColliderPtr->colliders.begin(); vecIter != aabbColliderPtr->colliders.end(); ++vecIter) {
			AABBCollider* collider = &(*vecIter);
			GameObject& go = aabbColliderPtr->GetGameObject();
			Transform& goTransform = go.GetComponent<Transform>();
			modelStack.PushMatrix();
				modelStack.Translate(goTransform.GetPosition().x + collider->centre.x,
									 goTransform.GetPosition().y + collider->centre.y,
									 goTransform.GetPosition().z + collider->centre.z);
				modelStack.Scale(collider->size.x, collider->size.y, collider->size.z);
				RenderHelper::GetInstance().RenderMesh(*aabbColliderMesh);
			modelStack.PopMatrix();
		}
	}

}