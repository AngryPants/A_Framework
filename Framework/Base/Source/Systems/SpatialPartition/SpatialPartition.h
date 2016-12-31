#ifndef SPACIAL_PARTITION_H
#define SPACIAL_PARTITION_H

#include "Grid.h"
#include "SingletonTemplate.h"

class SpatialPartition
{
private:
	// Variables
	string space;
	Grid* theGrid;
	int xSize, ySize, zSize;
	int xGridSize, yGridSize, zGridSize;
	int xNumOfGrid, yNumOfGrid, zNumOfGrid;
	int xPosition, yPosition, zPosition;
	
	// The vector of objects due for migration to another grid
	vector<GameObjectID> goList;
	
	void Sort(GameObjectID _gameObjectID);

public:
	// Constructor
	SpatialPartition(const string& _space);
	virtual ~SpatialPartition();
	// Initialise the spatial partition
	bool Set(const int _xGridSize, const int _yGridSize, const int _zGridSize,
			 const int _xNumOfGrid, const int _yNumOfGrid, const int _zNumOfGrid,
			 const int _xPosition = 0, const int _yPosition = 0, const int _zPosition = 0);
	// Update the spatial partition
	void Update();	

	// Get xSize of the entire spatial partition
	int GetxSize() const;
	// Get ySize of the entire spatial partition
	int GetySize() const;
	// Get zSize of the entire spatial partition
	int GetzSize() const;

	// Get xSize
	int GetxGridSize() const;
	// Get ySize
	int GetyGridSize() const;
	// Get zSize
	int GetzGridSize() const;
	
	int GetxPosition() const;
	int GetyPosition() const;
	int GetzPosition() const;

	// Get xNumOfGrid
	int GetxNumOfGrid() const;
	// Get yNumOfGrid
	int GetyNumOfGrid() const;
	// Get zNumOfGrid
	int GetzNumOfGrid() const; 

	// Get a particular grid
	Grid& GetGrid(const int _xIndex, const int _yIndex, const int _zIndex) const; 
	Grid& GetExtraGrid() const;
	vector<Grid*> GetSurroundingGrids(const int _xIndex, const int _yIndex, const int _zIndex);

	// Get vector of objects from this Spatial Partition
	vector<GameObjectID> GetObjects(Vector3 _position, const float _radius);

	// Add a new object
	void Add(GameObjectID _gameObjectID);
	// Remove but not delete object from this grid
	void Remove(GameObjectID _gameObjectID);

	//Calculate the distance from camera to a grid centrepoint
	float CalculateDistance(const Vector3& _theCameraPosition, const int _xIndex, const int _yIndex, const int _zIndex);
	// Calculate the squared distance from camera to a grid's centrepoint
	float CalculateDistanceSquare(const Vector3& theCameraPosition, const int _xIndex, const int _yIndex, const int _zIndex);

	// Set LOD distances
	//void SetLevelOfDetails(const float distance_High2Mid, const float distance_Mid2Low);
	
	// Check if a CGrid is visible to the camera
	bool IsVisible(Vector3 _theCameraPosition, Vector3 _theCameraDirection, const int _xIndex, const int _yIndex, const int _zIndex);
	
	// print the contents of all the grids
	void PrintSelf() const;
};


#endif