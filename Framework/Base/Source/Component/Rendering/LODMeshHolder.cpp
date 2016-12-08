#include "LODMeshHolder.h"
#include "../../Mesh/MeshBuilder.h"

bool LODMeshHolder::InitLOD(Mesh* mesh_High, Mesh* mesh_Mid, Mesh* mesh_Low)
{
	// If any of the mesh is not loaded, then return false and avoid setting the LOD to active
	if ((mesh[HIGH_DETAILS] == nullptr) || (mesh[MID_DETAILS] == nullptr) || (mesh[LOW_DETAILS] == nullptr))
		return false;

	// Retrieve the Meshes from MeshBuilder's library and assign to the 3 Mesh pointers
	mesh[HIGH_DETAILS] = mesh_High;
	mesh[MID_DETAILS] = mesh_Mid;
	mesh[LOW_DETAILS] = mesh_Low;

	return true;
}
 
bool LODMeshHolder::SetLODMesh(Mesh* theMesh, const LODMeshHolder::DETAIL_LEVEL theDetailLevel)
{
	if (theDetailLevel >= HIGH_DETAILS && theDetailLevel < NUM_DETAIL_LEVEL)
	{
		mesh[theDetailLevel] = theMesh; 
		return true;
	}

	return false;
}

//Mesh* LODMeshHolder::GetLODMesh() const
//{
//	if (theDetailLevel >= HIGH_DETAILS && theDetailLevel < NUM_DETAIL_LEVEL)
//		return mesh[theDetailLevel];
//
//	return NULL;
//}

Mesh* LODMeshHolder::GetLODMesh(const LODMeshHolder::DETAIL_LEVEL theDetailLevel) const
{
	if (theDetailLevel >= HIGH_DETAILS && theDetailLevel < NUM_DETAIL_LEVEL)
		return mesh[theDetailLevel];
	
	return NULL;
}

//
//int LODMeshHolder::GetDetailLevel() const
//{
//	return theDetailLevel;
//}
//
//bool LODMeshHolder::SetDetailLevel(const LODMeshHolder::DETAIL_LEVEL theDetailLevel)
//{
//	if ((theDetailLevel >= NO_DETAILS) && (theDetailLevel < NUM_DETAIL_LEVEL))
//	{
//		this->theDetailLevel = theDetailLevel;
//		return true;
//	}
//
//	return false;
//}