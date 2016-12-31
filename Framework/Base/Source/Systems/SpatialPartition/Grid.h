#ifndef GRID_H
#define GRID_H

#include "../../Component/Rendering/LODMeshHolder.h"
#include "../../Others/IDGenerator.h"
#include "GridIndex.h"

class Grid {

private:
	//We can find out position based on our min and max.
	int index;
	Vector3 position;
	Vector3 min, max;

public:
	vector<GameObjectID> goList;

	//Constructor(s) & Destructor
	Grid();
	virtual ~Grid();

	void Set(int _index, Vector3 _position, Vector3 _size);
	GridIndex GetIndex() const;
	Vector3 GetPosition() const;
	Vector3 GetMin() const;
	Vector3 GetMax() const;
			
	// Add a new object to this grid
	bool Add(GameObjectID _gameObjectID);
	// Remove but not delete all objects from this grid
	void RemoveAll();
	// Remove but not delete an object from this grid
	bool Remove(GameObjectID _gameObjectID);
	// Check if an object is in this grid
	bool IsHere(GameObjectID _gameObjectID) const;
	bool WithinGridBoundaries(const Vector3& _position) const;
	//Get the number of objects in this grid.
	int GetNumObjects() const;
	// print all the information it contains
	void PrintSelf();
};









#endif