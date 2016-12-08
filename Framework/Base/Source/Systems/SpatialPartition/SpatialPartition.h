#ifndef SPACIAL_PARTITION_H
#define SPACIAL_PARTITION_H

#include "Grid.h"
#include "SingletonTemplate.h"

class SpatialPartition
{
protected:

	// Variables
	Grid* theGrid;
	int xSize;
	int zSize;
	int xGridSize;
	int zGridSize;
	int xNumOfGrid;
	int zNumOfGrid;
	float yOffset;

	// LOD distances
	float LevelOfDetails_Distances[2];

public:

	// Constructor
	SpatialPartition();
	virtual ~SpatialPartition();
	// Initialise the spatial partition
	bool Init(const int xGridSize, const int zGridSize,
		const int xNumOfGrid, const int zNumOfGrid,
		const float yOffset = 0.f);
	// Update the spatial partition
	void Update();

	// Get xSize of the entire spatial partition
	int GetxSize() const;
	// Get zSize of the entire spatial partition
	int GetzSize() const;
	// Get xSize
	int GetxGridSize() const;
	// Get zNumOfGrid
	int GetzGridSize() const;
	// Get xNumOfGrid
	int GetxNumOfGrid() const;
	// Get zNumOfGrid
	int GetzNumOfGrid() const; 

	// Get a particular grid
	Grid GetGrid(const int xIndex, const int zIndex) const; 
	// Get vector of objects from this Spatial Partition
	vector<GameObjectID> GetObjects(Vector3 position, const float radius);

	// Add a new object
	void Add(GameObjectID theObject);
	// Remove but not delete object from this grid
	void Remove(GameObjectID theObject);
	
	// The vector of objects due for migration to another grid
	vector<GameObjectID> migrationList;

	//Calculate the distance from camera to a grid centrepoint
	float CalculateDistance(const Vector3& theCameraPosition, const int xIndex, const int zIndex);
	// Calculate the squared distance from camera to a grid's centrepoint
	float CalculateDistanceSquare(const Vector3& theCameraPosition, const int xIndex, const int zIndex);

	// Set LOD distances
	//void SetLevelOfDetails(const float distance_High2Mid, const float distance_Mid2Low);
	
	// Check if a CGrid is visible to the camera
	bool IsVisible(Vector3 theCameraPosition, Vector3 theCameraDirection, const int xIndex, const int zIndex);
	
	// print the contents of all the grids
	void PrintSelf() const;
};


#endif