#ifndef DEBUG_CONTROLS_SCRIPT_H
#define DEBUG_CONTROLS_SCRIPT_H

#include "../Script/Script.h"
#include "../Input/InputManager.h"
#include "../Systems/Rendering/RenderSystem.h"

class DebugControlsScript : public Script {

public:
	//Constructor(s) & Destructor
	DebugControlsScript(GameObject& gameObject): Script(gameObject) {
	}
	virtual ~DebugControlsScript() {}

	//Interface Function(s)
	virtual void Update(double deltaTime) {
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RENDER_SPATIAL_PARTITION_ON]) {
			RenderSystem::GetInstance().renderSpatialPartition = true;
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RENDER_SPATIAL_PARTITION_OFF]) {
			RenderSystem::GetInstance().renderSpatialPartition = false;
		}

		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RENDER_COLLIDERS_ON]) {
			RenderSystem::GetInstance().renderColliders = false;
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RENDER_COLLIDERS_OFF]) {
			RenderSystem::GetInstance().renderColliders = true;
		}
	}

};

#endif