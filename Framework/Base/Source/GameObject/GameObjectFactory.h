#ifndef GAMEOBJECT_FACTORY_H
#define GAMEOBJECT_FACTORY_H

#include "GameObject.h"

//Include Systems
#include "../Texture/TextureManager.h"
#include "../Mesh/MeshBuilder.h"

//Include Scripts

//Include Components
#include "../Component/Rendering/Camera.h"
#include "../Component/Rendering/Light.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Rendering/MeshRenderer.h"
#include "../Component/Rendering/SpriteRenderer.h"
#include "../Component/Rendering/TextRenderer.h"
#include "../Component/Physics/Collision/SphereCollider.h"

class GameObjectFactory {

public:
	GameObjectFactory() {}
	virtual ~GameObjectFactory() {}

	static GameObject& CreateCamera(const string& space, const string& name = "Camera") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Camera>();

		return go;
	}

	static GameObject& CreateLight(const string& space, const string& name = "Light") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Light>();

		return go;
	}

	static GameObject& CreateEmpty(const string& space, const string& name = "GameObject") {
		return GameObjectManager::GetInstance().CreateGameObject(space, name);
	}

	static GameObject& CreateCube(const string& space, const string& name = "Cube") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<MeshRenderer>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Default Cube", "OBJ//Default//Cube.obj");

		return go;
	}

};

#endif