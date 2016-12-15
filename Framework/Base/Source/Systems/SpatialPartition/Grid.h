#ifndef GRID_H
#define GRID_H

#include "../../GameObject/GameObject.h"
#include "../../Component/Rendering/LODMeshHolder.h"

struct GridIndex {
public:
	//Variable(s)
	int x, y, z;
	//Constructor(s) & Destructor
	GridIndex() {
		Set(0, 0, 0);
	}
	GridIndex(const int _x, const int _y, const int _z) {
		Set(_x, _y, _z);
	}
	GridIndex(const GridIndex& _rhs) {
		Set(_rhs.x, _rhs.y,_rhs.z);
	}
	~GridIndex() {}

	//Function(s)
	void Set(const int _x, const int _y, const int _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	//Operator Overloads
	GridIndex& operator=(const GridIndex& _rhs) {
		Set(_rhs.x, _rhs.y,_rhs.z);
		return *this;
	}
	bool operator==(const GridIndex& _rhs) const {
		return (this->x == _rhs.x) && (this->y == _rhs.y) && (this->z == _rhs.z);
	}

	//Friend Function(s)
	friend ostream& operator<<(ostream& os, const GridIndex& _rhs) {
		os << "[" << _rhs.x << ", " << _rhs.y << ", " << _rhs.z << "]";
		return os;
	}
};

class Grid
{
protected:
	// We use a Vector3 to store the indices of this Grid within the Spatial Partition array.
	GridIndex index;
	// We use a Vector3 to store the size of this Grid within the Spatial Partition array.
	Vector3 gridSize;
	// We use a Vector3 to store the x-y-z offset of the spatial partition.
	//Vector3 offset;
	//We can find out position based on our min and max.
	Vector3 position;
	// We use a Vector3 to store the x-y-z offset of this Grid.
	Vector3 min, max;

	//list of objects in this grid
	vector<GameObjectID> listOfObjects;
	
public:
	Grid();
	virtual ~Grid();
	// Init
	void Set(const GridIndex _index,
			 const int _xGridSize, const int _yGridSize, const int _zGridSize,
			 const int _xOffset, const int _yOffset, const int _zOffset);

	GridIndex GetIndex() const;
	Vector3 GetPosition() const;
	Vector3 GetMin() const;
	Vector3 GetMax() const;

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
	const vector<GameObjectID>& GetListOfObjects();

	//Get the number of objects in this grid.
	int GetNumObjects() const;

	// print all the information it contains
	void PrintSelf();
};









#endif