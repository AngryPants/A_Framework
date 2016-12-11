#include "SpatialPartition.h"
#include "../../Graphics/GraphicsManager.h"

SpatialPartition::SpatialPartition()
	: xSize(0), ySize(0), zSize(0)
	, xGridSize(0), yGridSize(0), zGridSize(0)
	, xNumOfGrid(0), yNumOfGrid(0), zNumOfGrid(0)
	, xPosition(0), yPosition(0), zPosition(0)
{
	theGrid = new Grid();
}

SpatialPartition::~SpatialPartition()
{
	//Delete our existing grids.
	if (this->xNumOfGrid * this->yNumOfGrid * this->zNumOfGrid > 0)
	{
		delete[] theGrid;
	}			
	else
	{
		delete theGrid;
	}
}

//initialise the size of the spatial partition and the number of partitions
bool SpatialPartition::Set(const int _xGridSize, const int _yGridSize, const int _zGridSize,
						   const int _xNumOfGrid, const int _yNumOfGrid, const int _zNumOfGrid,
						   const int _xPosition, const int _yPosition, const int _zPosition)
{
	if ((_xGridSize > 0) && (_yGridSize > 0) && (_zGridSize > 0) &&
		(_xNumOfGrid > 0) && (_yNumOfGrid > 0) && (_zNumOfGrid > 0))
	{
		//Get all the IDs from our existing grids.
		vector<GameObjectID> ids;		
		vector<GameObjectID> tempids;
		for (int i = 0; i < xNumOfGrid; i++)
		{
			for (int j = 0; j < yNumOfGrid; j++)
			{
				for (int k = 0; k < zNumOfGrid; k++)
				{
					tempids = theGrid[i * yNumOfGrid * zNumOfGrid + j * zNumOfGrid + k].GetListOfObjects();
					ids.insert(std::end(ids), std::begin(tempids), std::end(tempids));
				}
			}
		}
		//Get the IDs from our extra grid.
		tempids = theGrid[this->xNumOfGrid * this->yNumOfGrid * this->zNumOfGrid].GetListOfObjects();
		ids.insert(std::end(ids), std::begin(tempids), std::end(tempids));

		//Delete our existing grids.
		if (this->xNumOfGrid * this->yNumOfGrid * this->zNumOfGrid > 0)
		{
			delete[] theGrid;
		}
		else
		{
			delete theGrid;
		}
		
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
		theGrid = new Grid[this->xNumOfGrid * this->yNumOfGrid * this->zNumOfGrid + 1];

		//Initialise the new array of grids
		for (int i = 0; i < xNumOfGrid; ++i)
		{
			for (int j = 0; j < yNumOfGrid; ++j)
			{
				for (int k = 0; k < zNumOfGrid; ++k)
				{
					theGrid[(i * yNumOfGrid * zNumOfGrid) + (j * zNumOfGrid) + k].Set(GridIndex(i, j, k),
																					  xGridSize, yGridSize, zGridSize,
																					  xPosition - (xSize >> 1), yPosition - (ySize >> 1), zPosition - (zSize >> 1));
				}
			}
		}

		//Transfering the objects from previous spatial partition to current one
		for (vector<GameObjectID>::iterator iter = tempids.begin(); iter != tempids.end(); iter++)
		{
			Add(*iter);
		}

		// Create a migration list vector
		migrationList.clear();

		return true;
	}
	return false;
}

void SpatialPartition::Update()
{
	//for (int i = 0; i< xNumOfGrid; i++)
	//{
	//	for (int j = 0; j < zNumOfGrid; j++)
	//	{
	//		theGrid[i * zNumOfGrid + j].Update(&migrationList);
	//		/////////////////THIS CHECK SHOULD BE DONE IN RENDERSYSTEM////////////////////
	//		//// Check visibility
	//		//if (IsVisible(theCamera->GetCameraPos(),theCamera->GetCameraTarget() - theCamera->GetCameraPos(), i, j))
	//		//{
	//		//	// Calculate LOD for this CGrid
	//		//	float distance = CalculateDistanceSquare(&(theCamera->GetCameraPos()), i, j);
	//		//	if (distance < LevelOfDetails_Distances[0])
	//		//	{
	//		//		theGrid[i*zNumOfGrid + j].SetDetailLevel(LODMeshHolder::HIGH_DETAILS);
	//		//	}
	//		//	else if (distance < LevelOfDetails_Distances[1])
	//		//	{
	//		//		theGrid[i*zNumOfGrid + j].SetDetailLevel(LODMeshHolder::MID_DETAILS);
	//		//	}
	//		//	else
	//		//	{
	//		//		theGrid[i*zNumOfGrid + j].SetDetailLevel(LODMeshHolder::LOW_DETAILS);
	//		//	}
	//		//}
	//		//else
	//		//	theGrid[i*zNumOfGrid + j].SetDetailLevel(LODMeshHolder::NO_DETAILS);
	//	}
	//}

	for (int i = 0; i<xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			for (int k = 0; k<zNumOfGrid; k++)
			{
				theGrid[i * yNumOfGrid * zNumOfGrid + j * zNumOfGrid + k].Update(&migrationList);
			}
		}
	}
	theGrid[xNumOfGrid * yNumOfGrid * zNumOfGrid].Update(&migrationList); //Dude Teck Lee you forgot to update this one.

	// If there are objects due for migration, then process them
	if (!migrationList.empty())
	{
		// Check each object to see if they are no longer in this grid
		for (size_t i = 0; i < migrationList.size(); ++i)
		{
			Add(migrationList[i]);
		}

		migrationList.clear();
	}
}

/********************************************************************************
Get xSize of the entire spatial partition
********************************************************************************/
int SpatialPartition::GetxSize() const
{
	return xSize;
}
/********************************************************************************
Get ySize of the entire spatial partition
********************************************************************************/
int SpatialPartition::GetySize() const
{
	return ySize;
}
/********************************************************************************
Get zSize of the entire spatial partition
********************************************************************************/
int SpatialPartition::GetzSize() const
{
	return zSize;
}

/********************************************************************************
Get xSize
********************************************************************************/
int SpatialPartition::GetxGridSize() const
{
	return xGridSize;
}
/********************************************************************************
Get ySize
********************************************************************************/
int SpatialPartition::GetyGridSize() const
{
	return yGridSize;
}
/********************************************************************************
Get zNumOfGrid
********************************************************************************/
int SpatialPartition::GetzGridSize() const
{
	return zGridSize;
}

int SpatialPartition::GetxPosition() const
{
	return xPosition;
}

int SpatialPartition::GetyPosition() const 
{
	return yPosition;
}

int SpatialPartition::GetzPosition() const
{
	return zPosition;
}

/********************************************************************************
Get xNumOfGrid
********************************************************************************/
int SpatialPartition::GetxNumOfGrid() const
{
	return xNumOfGrid;
}
/********************************************************************************
Get yNumOfGrid
********************************************************************************/
int SpatialPartition::GetyNumOfGrid() const
{
	return yNumOfGrid;
}
/********************************************************************************
Get zNumOfGrid
********************************************************************************/
int SpatialPartition::GetzNumOfGrid() const
{
	return zNumOfGrid;
}

/********************************************************************************
Get a particular grid
********************************************************************************/
Grid SpatialPartition::GetGrid(const int _xIndex, const int _yIndex , const int _zIndex) const
{
	return theGrid[(_xIndex * yNumOfGrid * zNumOfGrid) + (_yIndex * zNumOfGrid) + _zIndex];
}

Grid SpatialPartition::GetExtraGrid() const {
	return theGrid[xNumOfGrid * yNumOfGrid * zNumOfGrid];
}

/********************************************************************************
Get vector of objects from this Spatial Partition (Incomplete)
********************************************************************************/
vector<GameObjectID> SpatialPartition::GetObjects(Vector3 position, const float radius)
{
	// Get the indices of the object's position
	int xIndex = (((int)position.x + xPosition - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int yIndex = (((int)position.y + yPosition - (-ySize >> 1)) / (ySize / yNumOfGrid));
	int zIndex = (((int)position.z + zPosition - (-zSize >> 1)) / (zSize / zNumOfGrid));

	return theGrid[(xIndex * yNumOfGrid * zNumOfGrid) + (yIndex * zNumOfGrid) + zIndex].GetListOfObjects();
}

/********************************************************************************
Add a new object model (Must take in account spatial partition position)
********************************************************************************/
void SpatialPartition::Add(GameObjectID theObject)
{
	GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(theObject);

	if (go == nullptr)
	{
		return;
	}	

	// Get the indices of the object's position
	int xIndex = -1;
	int yIndex = -1;
	int zIndex = -1;
	if (xNumOfGrid != 0 && yNumOfGrid != 0 && zNumOfGrid != 0)
	{
		xIndex = ((go->GetComponent<Transform>().GetPosition().x - xPosition - (-xSize >> 1)) / (xSize / xNumOfGrid));
		yIndex = ((go->GetComponent<Transform>().GetPosition().y - yPosition - (-ySize >> 1)) / (ySize / yNumOfGrid));
		zIndex = ((go->GetComponent<Transform>().GetPosition().z - zPosition - (-zSize >> 1)) / (zSize / zNumOfGrid));
	}
	
	// Add them to each grid
	if (((xIndex >= 0) && (xIndex < xNumOfGrid)) && 
		((yIndex >= 0) && (yIndex < yNumOfGrid)) &&
		((zIndex >= 0) && (zIndex < zNumOfGrid)))
	{
		theGrid[(xIndex * yNumOfGrid * zNumOfGrid) + (yIndex * zNumOfGrid) + zIndex].Add(theObject);
	}
	else
	{
		//Check for None initialized Spatial Partition
		theGrid[zNumOfGrid * yNumOfGrid * xNumOfGrid].Add(theObject);
	}
}

// Remove but not delete object from this grid
void SpatialPartition::Remove(GameObjectID theObject)
{
	GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(theObject);

	if (go == nullptr)
	{
		return;
	}

	// Get the indices of the object's position
	int xIndex = ((go->GetComponent<Transform>().GetPosition().x - xPosition - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int yIndex = ((go->GetComponent<Transform>().GetPosition().y - yPosition - (-ySize >> 1)) / (ySize / yNumOfGrid));
	int zIndex = ((go->GetComponent<Transform>().GetPosition().z - zPosition - (-zSize >> 1)) / (zSize / zNumOfGrid));

	// Add them to each grid
	if (((xIndex >= 0) && (xIndex<xNumOfGrid)) &&
		((yIndex >= 0) && (yIndex<yNumOfGrid)) &&
		((zIndex >= 0) && (zIndex<zNumOfGrid)))
	{
		theGrid[(xIndex * yNumOfGrid * zNumOfGrid) + (yIndex * zNumOfGrid) + zIndex].Remove(theObject);
	}
	
}

/********************************************************************************
Calculate the squared distance from camera to a grid's centrepoint
********************************************************************************/
float SpatialPartition::CalculateDistanceSquare(const Vector3& _theCameraPosition, const int _xIndex, const int _yIndex, const int _zIndex)
{
	/*float xDistance = (xGridSize*_xIndex + (xGridSize >> 1) - (xSize >> 1)) - _theCameraPosition.x;
	float yDistance = (yGridSize*_xIndex + (yGridSize >> 1) - (ySize >> 1)) - _theCameraPosition.y;
	float zDistance = (zGridSize*_zIndex + (zGridSize >> 1) - (zSize >> 1)) - _theCameraPosition.z;

	return (float)(xDistance*xDistance + yDistance*yDistance +  zDistance*zDistance);*/

	return (GetGrid(_xIndex, _yIndex, _zIndex).GetPosition() - _theCameraPosition).LengthSquared();
}

float SpatialPartition::CalculateDistance(const Vector3& _theCameraPosition, const int _xIndex, const int _yIndex, const int _zIndex)
{
	//return sqrt(CalculateDistanceSquare(_theCameraPosition, _xIndex, _yIndex, _zIndex));
	return (GetGrid(_xIndex, _yIndex, _zIndex).GetPosition() - _theCameraPosition).Length();
}

/********************************************************************************
Set LOD distances
********************************************************************************/
//void SpatialPartition::SetLevelOfDetails(const float distance_High2Mid, const float distance_Mid2Low)
//{
//	LevelOfDetails_Distances[0] = distance_High2Mid;
//	LevelOfDetails_Distances[1] = distance_Mid2Low;
//}

/********************************************************************************
Check if a CGrid is visible to the camera
********************************************************************************/
bool SpatialPartition::IsVisible(Vector3 _theCameraPosition, Vector3 _theCameraDirection, const int _xIndex, const int _yIndex, const int _zIndex)
{
	Grid& grid = GetGrid(_xIndex, _yIndex, _zIndex);
	Vector3 min = grid.GetMin();
	Vector3 max = grid.GetMax();
	if (_theCameraPosition.x >= min.x && _theCameraPosition.y >= min.y && _theCameraPosition.z >= min.z &&
		_theCameraPosition.x <= max.x && _theCameraPosition.y <= max.y && _theCameraPosition.z <= max.z)
	{		
		return true;
	}
	if (_theCameraDirection.Dot(min) <= 0.0f && _theCameraDirection.Dot(max) <= 0.0f)
	{
		return false;
	}

	return true;
}

/********************************************************************************
PrintSelf
********************************************************************************/
void SpatialPartition::PrintSelf() const
{
	cout << endl;
	cout << "**************** Start of SpatialPartition::PrintSelf() ********************" << endl;
	cout << "xSize\t:\t" << xSize << "\tySize\t:\t" << ySize << "\tzSize\t:\t" << zSize << endl;
	cout << "xNumOfGrid\t:\t" << xNumOfGrid << "\tyNumOfGrid\t:\t" << yNumOfGrid << "\tzNumOfGrid\t:\t" << zNumOfGrid << endl;
	if (theGrid)
	{
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j < yNumOfGrid; j++)
			{
				for (int k = 0; k<zNumOfGrid; k++)
				{
					theGrid[(i * yNumOfGrid * zNumOfGrid) + (j * zNumOfGrid) + k].PrintSelf();
				}
			}
		}
		theGrid[xNumOfGrid * yNumOfGrid * zNumOfGrid].PrintSelf();
	}
	else
	{
		cout << "theGrid : NULL" << endl;
	}		
	cout << "******* End of SpatialPartition::PrintSelf() **********************************" << endl;
	cout << endl;
}
