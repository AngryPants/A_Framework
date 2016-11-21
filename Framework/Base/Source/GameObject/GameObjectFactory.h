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

	static GameObject& CreateSphere(const string& space, const string& name = "Sphere") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<MeshRenderer>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Default Sphere", "OBJ//Default//Sphere.obj");

		return go;
	}

	static GameObject& CreateQuad(const string& space, const string& name = "Quad") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<MeshRenderer>().mesh = MeshBuilder::GetInstance().GenerateQuad("Default Quad");

		return go;
	}

	static GameObject& CreatePlane(const string& space, const string& name = "Plane") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<MeshRenderer>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Default Plane", "OBJ//Default//Plane.obj");

		return go;
	}

	static GameObject& CreateCylinder(const string& space,const string& name = "Cylinder") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space,name);
		go.AddComponent<MeshRenderer>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Default Cylinder","OBJ//Default//Cylinder.obj");

		return go;
	}

};

#endif