#include "CutsceneTriggerScript.h"
#include "../GameObject/GameObjectFactory.h"
#include "../AGDEV/PivotScript.h"

//Constructor(s) & Destructor
CutsceneTriggerScript::CutsceneTriggerScript(GameObject& gameObject) : Script(gameObject) {
	timer = 4.0f;
	play = false;

	camPivot = &GameObjectManager::GetInstance().CreateGameObject(GetGameObject().GetSpace(), "Camera Pivot");
	camPivot->SetParent(GetGameObject());
	camPivot->CreateScript<PivotScript>();
	
	camera = &GameObjectFactory::CreateCamera(GetGameObject().GetSpace(), "Cutscene Camera");
	camera->SetParent(*camPivot);
	camera->GetComponent<Transform>().SetLocalPosition(0, 3, -3);
	camera->GetComponent<Transform>().SetLocalRotation(30, 0, 0);
	camera->GetComponent<Camera>().depth = 10.0f;
	camera->GetComponent<Camera>().SetInActive();
}

CutsceneTriggerScript::~CutsceneTriggerScript() {
}

//Interface Function(s)
void CutsceneTriggerScript::Update(double deltaTime) {
	if (play) {		
		timer -= deltaTime;
		if (timer > 0.0f) {
			camera->GetComponent<Camera>().SetActive();			
		} else {
			camera->GetComponent<Camera>().SetInActive();
			play = false;
		}
	}
}

void CutsceneTriggerScript::OnTriggerStay(const Collider& _collider) {
	GameObject* player = GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID);
	if (player != nullptr && player->name == "Player") {
		play = true;
	}
}