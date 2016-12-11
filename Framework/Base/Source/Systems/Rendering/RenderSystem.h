#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "SingletonTemplate.h"
#include "../../Others/IDGenerator.h"
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
	void RenderRecursion(GameObject& gameObject);	
	//Debuging Stuff
	void RenderGridBoundaries(const string& _space);
	void RenderGrid(Grid& grid, LODMeshHolder::DETAIL_LEVEL detailLevel);

	//Debugging Stuff
	Mesh* gridMesh;
	Mesh* gridMeshEmpty;

public:
	//Interface Function(s)
	virtual void Update(const string& space, double deltaTime);
	virtual void Render(const string& space);
	virtual void RenderUI(const string& space);
	
};

#endif