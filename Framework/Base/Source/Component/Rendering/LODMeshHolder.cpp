#include "LODMeshHolder.h"
#include "MeshHolder.h"
#include "MeshRenderer.h"
#include "../../Mesh/MeshBuilder.h"
#include "../ComponentManager.h"

//Constructor(s) & Destructor
LODMeshHolder::LODMeshHolder(GameObject& _gameObject, PassKey<ComponentManager> _key) : Component("LOD Mesh Holder", _gameObject, _key)
{
	for (int i = 0; i < NUM_DETAIL_LEVEL; i++)
	{
		mesh[i] = nullptr;
	}

	if (GetGameObject().HasComponent<MeshHolder>())
	{
		GetGameObject().RemoveComponent<MeshHolder>();
	}
	if (!GetGameObject().HasComponent<MeshRenderer>())
	{
		GetGameObject().AddComponent<MeshRenderer>();
	}	
}

LODMeshHolder::~LODMeshHolder() {
}

//Mesh
bool LODMeshHolder::SetLODMesh(Mesh* _meshLow, Mesh* _meshMid, Mesh* _meshHigh)
{
	//If any of the mesh is not loaded, then return false and avoid setting the LOD to active
	/*if ((mesh[HIGH_DETAILS] == nullptr) || (mesh[MID_DETAILS] == nullptr) || (mesh[LOW_DETAILS] == nullptr))
	{
		return false;
	}*/

	mesh[LOW_DETAILS] = _meshLow;
	mesh[MID_DETAILS] = _meshMid;
	mesh[HIGH_DETAILS] = _meshHigh;

	return true;
}
 
bool LODMeshHolder::SetLODMesh(Mesh* _mesh, const LODMeshHolder::DETAIL_LEVEL _detailLevel)
{
	if (_detailLevel < NUM_DETAIL_LEVEL)
	{
		mesh[_detailLevel] = _mesh;
		return true;
	}

	return false;
}

bool LODMeshHolder::RemoveMesh(const DETAIL_LEVEL _detailLevel)
{
	if (_detailLevel < NUM_DETAIL_LEVEL)
	{
		mesh[_detailLevel] = nullptr;
		return true;
	}

	return false;
}

bool LODMeshHolder::RemoveAllMeshes()
{
	for (unsigned int i = 0; i < NUM_DETAIL_LEVEL; ++i) {
		mesh[i] = nullptr;
	}

	return true;
}

Mesh* LODMeshHolder::GetLODMesh(const LODMeshHolder::DETAIL_LEVEL _detailLevel)
{
	if (_detailLevel < NUM_DETAIL_LEVEL)
	{
		return mesh[_detailLevel];
	}		
	
	return nullptr;
}

//Textures
bool LODMeshHolder::SetLODTextures(const TextureList& _textureListLow, const TextureList& _textureListMid, const TextureList& _textureListHigh)
{
	textureList[LOW_DETAILS] = _textureListLow;
	textureList[MID_DETAILS] = _textureListMid;
	textureList[HIGH_DETAILS] = _textureListHigh;

	return true;
}

bool LODMeshHolder::SetLODTextures(const TextureList& _textureList, const DETAIL_LEVEL _detailLevel)
{
	if (_detailLevel < NUM_DETAIL_LEVEL)
	{
		textureList[_detailLevel] = _textureList;
		return true;
	}

	return false;
}

bool LODMeshHolder::RemoveTextures(const DETAIL_LEVEL _detailLevel)
{
	if (_detailLevel < NUM_DETAIL_LEVEL)
	{
		textureList[_detailLevel].Reset();
		return true;
	}

	return false;
}

bool LODMeshHolder::RemoveAllTextures()
{
	for (unsigned int i = 0; i < NUM_DETAIL_LEVEL; ++i) {
		textureList[i].Reset();
	}

	return true;
}

TextureList& LODMeshHolder::GetTextureList(const DETAIL_LEVEL _detailLevel) {
	if (_detailLevel < NUM_DETAIL_LEVEL)
	{
		return textureList[_detailLevel];
	}		
	
	return TextureList();
}