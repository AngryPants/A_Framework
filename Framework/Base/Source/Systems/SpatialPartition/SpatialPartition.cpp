#include "SpatialPartition.h"
#include "../../Graphics/GraphicsManager.h"

SpatialPartition::SpatialPartition(const string& _space)
	: space(_space)
	, xSize(0), ySize(0), zSize(0)
	, xGridSize(0), yGridSize(0), zGridSize(0)
	, xNumOfGrid(0), yNumOfGrid(0), zNumOfGrid(0)
	, xPosition(0), yPosition(0), zPosition(0)
{
	theGrid = new Grid[1];
}

SpatialPartition::~SpatialPartition() {
	//Delete our existing grids.
	delete[] theGrid;
}

//initialise the size of the spatial partition and the number of partitions
bool SpatialPartition::Set(const int _xGridSize, const int _yGridSize, const int _zGridSize,
						   const int _xNumOfGrid, const int _yNumOfGrid, const int _zNumOfGrid,
						   const int _xPosition, const int _yPosition, const int _zPosition)
{
	if (_xGridSize < 0 || _yGridSize < 0 || _zGridSize < 0 ||
		_xNumOfGrid < 0 || _yNumOfGrid < 0 || _zNumOfGrid < 0) {
		return false;
	}
	
	//Delete our existing grids.
	delete[] theGrid;

	//Update our new thingies.
	this->xNumOfGrid = _xNumOfGrid;
	this->yNumOfGrid = _yNumOfGrid;
	this->zNumOfGrid = _zNumOfGrid;

	this->xGridSize = _xGridSize;
	this->yGridSize = _yGridSize;
	this->zGridSize = _zGridSize;

	this->xPosition = _xPosition;
	this->yPosition = _yPosition;
	this->zPosition = _zPosition;

	this->xSize = _xGridSize * _xNumOfGrid;
	this->ySize = _yGridSize * _yNumOfGrid;
	this->zSize = _zGridSize * _zNumOfGrid;

	//Create an array of grids
	theGrid = new Grid[xNumOfGrid * yNumOfGrid * zNumOfGrid + 1];

	Vector3 gridSize(xGridSize, yGridSize, zGridSize);
	Vector3 offset(xPosition - (xSize >> 1), yPosition - (ySize >> 1), zPosition - (zSize >> 1));
	//Initialise the new array of grids.
	for (int x = 0; x < xNumOfGrid; ++x) {
		for (int y = 0; y < yNumOfGrid; ++y) {
			for (int z = 0; z < zNumOfGrid; ++z) {
				GridIndex gridIndex = (x * yNumOfGrid * zNumOfGrid) + (y * zNumOfGrid) + z;
				Vector3 gridPosition = offset + Vector3(x * xGridSize, y * yGridSize, z * zGridSize) + (gridSize * 0.5f);
				theGrid[gridIndex].Set(gridIndex, gridPosition, gridSize);
			}
		}
	}
	
	Update();

	return true;
}

void SpatialPartition::Update() {
	//Clear the goList of every grid.
	for (int x = 0; x < xNumOfGrid; ++x) {
		for (int y = 0; y < yNumOfGrid; ++y) {
			for (int z = 0; z < zNumOfGrid; ++z) {
				theGrid[(x * yNumOfGrid * zNumOfGrid) + (y * zNumOfGrid) + z].goList.clear();
			}
		}
	}
	theGrid[xNumOfGrid * yNumOfGrid * zNumOfGrid].goList.clear();

	for (unsigned int i = 0; i < goList.size(); ++i) {
		Sort(goList[i]);
	}
}

/********************************************************************************
Get xSize of the entire spatial partition
********************************************************************************/
int SpatialPartition::GetxSize() const {
	return xSize;
}
/********************************************************************************
Get ySize of the entire spatial partition
********************************************************************************/
int SpatialPartition::GetySize() const {
	return ySize;
}
/********************************************************************************
Get zSize of the entire spatial partition
********************************************************************************/
int SpatialPartition::GetzSize() const {
	return zSize;
}

/********************************************************************************
Get xSize
********************************************************************************/
int SpatialPartition::GetxGridSize() const {
	return xGridSize;
}
/********************************************************************************
Get ySize
********************************************************************************/
int SpatialPartition::GetyGridSize() const {
	return yGridSize;
}
/********************************************************************************
Get zNumOfGrid
********************************************************************************/
int SpatialPartition::GetzGridSize() const {
	return zGridSize;
}

int SpatialPartition::GetxPosition() const {
	return xPosition;
}

int SpatialPartition::GetyPosition() const {
	return yPosition;
}

int SpatialPartition::GetzPosition() const {
	return zPosition;
}

/********************************************************************************
Get xNumOfGrid
********************************************************************************/
int SpatialPartition::GetxNumOfGrid() const {
	return xNumOfGrid;
}
/********************************************************************************
Get yNumOfGrid
********************************************************************************/
int SpatialPartition::GetyNumOfGrid() const {
	return yNumOfGrid;
}
/********************************************************************************
Get zNumOfGrid
********************************************************************************/
int SpatialPartition::GetzNumOfGrid() const {
	return zNumOfGrid;
}

/********************************************************************************
Get a particular grid
********************************************************************************/
Grid& SpatialPartition::GetGrid(const int _xIndex, const int _yIndex , const int _zIndex) const {
	return theGrid[(_xIndex * yNumOfGrid * zNumOfGrid) + (_yIndex * zNumOfGrid) + _zIndex];
}

Grid& SpatialPartition::GetExtraGrid() const {
	return theGrid[xNumOfGrid * yNumOfGrid * zNumOfGrid];
}

vector<Grid*> SpatialPartition::GetSurroundingGrids(const int _xIndex, const int _yIndex, const int _zIndex) {

	vector<Grid*> result;
	//Start From Bottom and Bottom Left
	for (unsigned int x = 0; x < 3; ++x) {
		for (unsigned int y = 0; y < 3; ++y) {
			for (unsigned int z = 0; z < 3; ++z) {
				int xIndex = _xIndex - 1 + x;
				int yIndex = _yIndex - 1 + y;
				int zIndex = _zIndex - 1 + z;

				//If it is the centre grid, ignore it.
				if (xIndex == _xIndex && yIndex == _yIndex && zIndex == _zIndex) {
					continue;
				}

				//If it is not within our entire spatial partition, ignore it
				if (xIndex < 0 || xIndex >= xNumOfGrid ||
					yIndex < 0 || yIndex >= yNumOfGrid ||
					zIndex < 0 || zIndex >= zNumOfGrid) {
					continue;
				}

				result.push_back(&GetGrid(xIndex, yIndex, zIndex));
			}
		}		
	}

	return result;
}

/********************************************************************************
Get vector of objects from this Spatial Partition (Incomplete)
********************************************************************************/
vector<GameObjectID> SpatialPartition::GetObjects(Vector3 position, const float radius) {
	// Get the indices of the object's position
	int xIndex = (((int)position.x + xPosition - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int yIndex = (((int)position.y + yPosition - (-ySize >> 1)) / (ySize / yNumOfGrid));
	int zIndex = (((int)position.z + zPosition - (-zSize >> 1)) / (zSize / zNumOfGrid));

	return theGrid[(xIndex * yNumOfGrid * zNumOfGrid) + (yIndex * zNumOfGrid) + zIndex].goList;
}

/********************************************************************************
Add a new object model (Must take in account spatial partition position)
********************************************************************************/
void SpatialPartition::Add(GameObjectID _gameObjectID) {
	goList.push_back(_gameObjectID);
	Sort(_gameObjectID);
}

void SpatialPartition::Sort(GameObjectID _gameObjectID) {
	//This function assumes that theObject is always valid.
	GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(_gameObjectID);
	//If it doesn't use spatial paritioning, put it in the extra grid.
	if (go->useSpatialPartition == false) {
		theGrid[zNumOfGrid * yNumOfGrid * xNumOfGrid].Add(_gameObjectID);
		return;
	}

	// Get the indices of the object's position
	int xIndex = -1;
	int yIndex = -1;
	int zIndex = -1;

	//Handle the special case where 0 to -1 has issues do to float to int conversion.
	float xIndex_Float = ((go->GetComponent<Transform>().GetPosition().x - xPosition + (xSize >> 1)) / xGridSize);
	float yIndex_Float = ((go->GetComponent<Transform>().GetPosition().y - yPosition + (ySize >> 1)) / yGridSize);
	float zIndex_Float = ((go->GetComponent<Transform>().GetPosition().z - zPosition + (zSize >> 1)) / zGridSize);
	if (xIndex_Float >= 0.0f) {
		xIndex = static_cast<int>(xIndex_Float);
	}
	if (yIndex_Float >= 0.0f) {
		yIndex = static_cast<int>(yIndex_Float);
	}
	if (zIndex_Float >= 0.0f) {
		zIndex = static_cast<int>(zIndex_Float);
	}

	if (xNumOfGrid * yNumOfGrid * zNumOfGrid > 0) {
		xIndex = ((go->GetComponent<Transform>().GetPosition().x - xPosition - (-xSize >> 1)) / (xSize / xNumOfGrid));
		yIndex = ((go->GetComponent<Transform>().GetPosition().y - yPosition - (-ySize >> 1)) / (ySize / yNumOfGrid));
		zIndex = ((go->GetComponent<Transform>().GetPosition().z - zPosition - (-zSize >> 1)) / (zSize / zNumOfGrid));
	}

	// Add them to each grid
	if (((xIndex >= 0) && (xIndex < xNumOfGrid)) &&
		((yIndex >= 0) && (yIndex < yNumOfGrid)) &&
		((zIndex >= 0) && (zIndex < zNumOfGrid)))
	{
		theGrid[(xIndex * yNumOfGrid * zNumOfGrid) + (yIndex * zNumOfGrid) + zIndex].Add(_gameObjectID);
	} else {
		theGrid[zNumOfGrid * yNumOfGrid * xNumOfGrid].Add(_gameObjectID);
	}
}

//Remove but not delete object from this grid
void SpatialPartition::Remove(GameObjectID theObject) {
	GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(theObject);
	GridIndex gridIndex = go->GetGridIndex();
	if (gridIndex == -1) {
		theGrid[xNumOfGrid * yNumOfGrid * zNumOfGrid].Remove(theObject);
	} else {
		theGrid[gridIndex].Remove(theObject);
	}	

	for (vector<GameObjectID>::iterator vecIter = goList.begin(); vecIter != goList.end(); ++vecIter) {
		if ((*vecIter) == theObject) {
			goList.erase(vecIter);
			break;
		}
	}
}

/********************************************************************************
Calculate the squared distance from camera to a grid's centrepoint
********************************************************************************/
float SpatialPartition::CalculateDistanceSquare(const Vector3& _theCameraPosition, const int _xIndex, const int _yIndex, const int _zIndex) {
	return (GetGrid(_xIndex, _yIndex, _zIndex).GetPosition() - _theCameraPosition).LengthSquared();
}

float SpatialPartition::CalculateDistance(const Vector3& _theCameraPosition, const int _xIndex, const int _yIndex, const int _zIndex) {
	return (GetGrid(_xIndex, _yIndex, _zIndex).GetPosition() - _theCameraPosition).Length();
}

/********************************************************************************
Check if a CGrid is visible to the camera
********************************************************************************/
bool SpatialPartition::IsVisible(Vector3 _theCameraPosition, Vector3 _theCameraDirection, const int _xIndex, const int _yIndex, const int _zIndex) {
	Grid& grid = GetGrid(_xIndex, _yIndex, _zIndex);
	Vector3 min = grid.GetMin();
	Vector3 max = grid.GetMax();
	if (_theCameraPosition.x >= min.x && _theCameraPosition.y >= min.y && _theCameraPosition.z >= min.z &&
		_theCameraPosition.x <= max.x && _theCameraPosition.y <= max.y && _theCameraPosition.z <= max.z)
	{
		return true;
	}
	if (_theCameraDirection.Dot(min - _theCameraPosition) > 0.0f && _theCameraDirection.Dot(max - _theCameraPosition) > 0.0f)
	{
		return true;
	}
	
	if ((grid.GetPosition() - _theCameraPosition).LengthSquared() < Vector3(xGridSize, yGridSize, zGridSize).LengthSquared()) {
		return true;
	}

	return false;
}

/********************************************************************************
PrintSelf
********************************************************************************/
void SpatialPartition::PrintSelf() const {
	cout << endl;
	cout << "**************** Start of SpatialPartition::PrintSelf() ********************" << endl;
	cout << "xSize\t:\t" << xSize << "\tySize\t:\t" << ySize << "\tzSize\t:\t" << zSize << endl;
	cout << "xNumOfGrid\t:\t" << xNumOfGrid << "\tyNumOfGrid\t:\t" << yNumOfGrid << "\tzNumOfGrid\t:\t" << zNumOfGrid << endl;
	if (theGrid) {
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i = 0; i<xNumOfGrid; i++) {
			for (int j = 0; j < yNumOfGrid; j++) {
				for (int k = 0; k<zNumOfGrid; k++) {
					theGrid[(i * yNumOfGrid * zNumOfGrid) + (j * zNumOfGrid) + k].PrintSelf();
				}
			}
		}
		theGrid[xNumOfGrid * yNumOfGrid * zNumOfGrid].PrintSelf();
	} else {
		cout << "theGrid : NULL" << endl;
	}		
	cout << "******* End of SpatialPartition::PrintSelf() **********************************" << endl;
	cout << endl;
}
