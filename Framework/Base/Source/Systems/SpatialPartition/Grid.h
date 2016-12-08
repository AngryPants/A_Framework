#ifndef GRID_H
#define GRID_H

#include "../../GameObject/GameObject.h"
#include "../../Component/Rendering/LODMeshHolder.h"

class Grid
{
protected:
	// We use a Vector3 to store the indices of this Grid within the Spatial Partition array.
	Vector3 index;
	// We use a Vector3 to store the size of this Grid within the Spatial Partition array.
	Vector3 size;
	// We use a Vector3 to store the x- and z-offset of this Grid.
	Vector3 offset;
	// We use a Vector3 to store the x- and z-offset of this Grid.
	Vector3 min, max;

	//list of objects in this grid
	vector<GameObjectID> listOfObjects;
	
public:
	Grid();
	~Grid();
	// Init
	void Init(const int xIndex, const int zIndex,
		const int xGridSize, const int zGridSize,
		const float xOffset = 0, const float zOffset = 0);

	// Update the grid
	void Update(vector<GameObjectID>* migrationList);
	
	// Add a new object to this grid
	bool Add(GameObjectID theObject);
	// Remove but not delete all objects from this grid
	void RemoveAll();
	// Remove but not delete an object from this grid
	bool Remove(GameObjectID theObject);

	// Check if an object is in this grid
	bool IsHere(GameObjectID theObject) const;

	// Get list of objects in this grid
	vector<GameObjectID> GetListOfObject();

	// print all the information it contains
	void PrintSelf();
};









#endif