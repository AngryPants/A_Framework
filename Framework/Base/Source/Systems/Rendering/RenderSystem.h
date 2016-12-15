#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "SingletonTemplate.h"
#include "../../Others/IDGenerator.h"
#include "../../Texture/TextureList.h"
#include "../../Others/Skybox.h"
#include <string>
#include <bitset>

//Include Components
#include "../../Component/Physics/Transform.h"
#include "../../Component/Rendering/Camera.h"
#include "../../Component/Rendering/Light.h"
#include "../../Component/Rendering/MeshRenderer.h"
#include "../../Component/Rendering/MeshHolder.h"
#include "../../Component/Rendering/LODMeshHolder.h"
#include "../../Component/Rendering/SpriteRenderer.h"
#include "../../Component/Rendering/TextRenderer.h"
#include "../../Component/Physics/Collision/ColliderGroup.h"

//Include Colliders
#include "../../Collider/AABBCollider.h"
#include "../../Collider/SphereCollider.h"

using namespace std;

class Mesh;
class SpatialPartition;
class Grid;

class RenderSystem : public Singleton<RenderSystem> {

	friend class Singleton<RenderSystem>;

private:
	//Constructor(s) & Destructor
	RenderSystem();
	virtual ~RenderSystem();

	//Private Function(s)
	void RenderGrid(Grid& grid, LODMeshHolder::DETAIL_LEVEL detailLevel);
	//Debuging Stuff
	void RenderGridBoundaries(const string& _space);	
	void RenderColliders(const string& _space);
	void RenderUI(TextRenderer& textRenderer);

	//Skybox
	void RenderSkybox(const Vector3& position, const Skybox* _skybox);	

	//Debugging Stuff
	Mesh* gridMesh;
	Mesh* gridMeshEmpty;
	Mesh* sphereColliderMesh;
	Mesh* aabbColliderMesh;

public:	
	//Variable(s)
	bool renderSpatialPartition;
	bool renderColliders;

	//Interface Function(s)
	virtual void Update(const string& space, double deltaTime);
	virtual void Render(const string& _space, const Skybox* _skybox = nullptr);
	
	
};

#endif