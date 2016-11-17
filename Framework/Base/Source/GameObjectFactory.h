#ifndef GAMEOBJECT_FACTORY_H
#define GAMEOBJECT_FACTORY_H

#include "GameObject.h"

//Include Systems
#include "TextureManager.h"
#include "MeshBuilder.h"

//Include Scripts

//Include Components
#include "Camera.h"
#include "Transform.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "HealthComponent.h"
#include "SphereCollider.h"

class GameObjectFactory {

public:
	GameObjectFactory() {}
	virtual ~GameObjectFactory() {}

	static GameObject& CreateCamera(const string& space, const string& name = "Camera") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Camera>();
		go.AddComponent<Transform>();

		return go;
	}

	static GameObject& CreateLight(const string& space, const string& name = "Light") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Light>();
		go.AddComponent<Transform>();

		return go;
	}

	static GameObject& CreateEmpty(const string& space, const string& name = "GameObject") {
		return GameObjectManager::GetInstance().CreateGameObject(space, name);
	}

};

#endif