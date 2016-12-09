#include "SpatialPartition.h"
#include "../../Graphics/GraphicsManager.h"

SpatialPartition::SpatialPartition()
	: xSize(0)
	, zSize(0)
	, xGridSize(0)
	, zGridSize(0)
	, xNumOfGrid(0)
	, zNumOfGrid(0)
{
	theGrid = new Grid();
}

SpatialPartition::~SpatialPartition()
{
	delete[] theGrid;
}

//initialise the size of the spatial partition and the number of partitions
bool SpatialPartition::Init(const int xGridSize, const int yGridSize, const int zGridSize,
	const int xNumOfGrid, const int yNumOfGrid, const int zNumOfGrid)
{
	if ((xGridSize>0) && (yGridSize>0) && (zGridSize>0) &&
		(xNumOfGrid>0) && (yGridSize>0) && (zNumOfGrid>0))
	{
		vector<GameObjectID> ids;
		for (int i = 0; i < this->xNumOfGrid; i++)
		{
			for (int k = 0; k < this->yNumOfGrid; k++)
			{
				for (int j = 0; j < this->zNumOfGrid; j++)
				{
					vector<GameObjectID> tempids = theGrid[i * k * this->zNumOfGrid + j].GetListOfObject();
					ids.insert(std::end(ids), std::begin(tempids), std::end(tempids));
				}
			}
		}
		vector<GameObjectID> tempids = theGrid[this->xNumOfGrid * this->yNumOfGrid * this->zNumOfGrid].GetListOfObject();
		ids.insert(std::end(ids), std::begin(tempids), std::end(tempids));
		if (this->xNumOfGrid * this->yNumOfGrid * this->zNumOfGrid > 0)
			delete[] theGrid;
		else
			delete theGrid;

		this->xNumOfGrid = xNumOfGrid;
		this->yNumOfGrid = yNumOfGrid;
		this->zNumOfGrid = zNumOfGrid;
		this->xGridSize = xGridSize;
		this->yGridSize = yGridSize;
		this->zGridSize = zGridSize;
		this->xSize = xGridSize * xNumOfGrid;
		this->ySize = yGridSize * yNumOfGrid;
		this->zSize = zGridSize * zNumOfGrid;

		// Create an array of grids
		theGrid = new Grid[xNumOfGrid * yNumOfGrid * zNumOfGrid + 1];

		// Initialise the array of grids
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int k = 0; k < yNumOfGrid; k++)
			{
				for (int j = 0; j<zNumOfGrid; j++)
				{
					theGrid[i * k * zNumOfGrid + j].Init(i, k, j, xGridSize, yNumOfGrid, zGridSize, (xSize >> 1), (ySize>>1), (zSize >> 1));
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
		for (int k = 0; k < yNumOfGrid; k++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i * zNumOfGrid + k *yNumOfGrid + j].Update(&migrationList);
			}
		}
	}

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
Grid SpatialPartition::GetGrid(const int xIndex, const int yIndex , const int zIndex) const
{
	return theGrid[xIndex * yIndex * zNumOfGrid + zIndex];
}

/********************************************************************************
Get vector of objects from this Spatial Partition
********************************************************************************/
vector<GameObjectID> SpatialPartition::GetObjects(Vector3 position, const float radius)
{
	// Get the indices of the object's position
	int xIndex = (((int)position.x - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int yIndex = (((int)position.y - (-ySize >> 1)) / (ySize / yNumOfGrid));
	int zIndex = (((int)position.z - (-zSize >> 1)) / (zSize / zNumOfGrid));

	return theGrid[xIndex * yIndex * zNumOfGrid + zIndex].GetListOfObject();
}

/********************************************************************************
Add a new object model
********************************************************************************/
void SpatialPartition::Add(GameObjectID theObject)
{
	GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(theObject);

	if (go == nullptr)
		return;

	// Get the indices of the object's position
	int xIndex = -1;
	int yIndex = -1;
	int zIndex = -1;
	if (xNumOfGrid != 0 && yNumOfGrid != 0 && zNumOfGrid != 0)
	{
		xIndex = ((go->GetComponent<Transform>().GetPosition().x - (-xSize >> 1)) / (xSize / xNumOfGrid));
		yIndex = ((go->GetComponent<Transform>().GetPosition().y - (-ySize >> 1)) / (ySize / yNumOfGrid));
		zIndex = ((go->GetComponent<Transform>().GetPosition().z - (-zSize >> 1)) / (zSize / zNumOfGrid));
	}
	
	// Add them to each grid
	if (((xIndex >= 0) && (xIndex<xNumOfGrid)) && 
		((yIndex >= 0) && (yIndex<yNumOfGrid)) &&
		((zIndex >= 0) && (zIndex<zNumOfGrid)))
	{
		theGrid[xIndex * yIndex * zNumOfGrid + zIndex].Add(theObject);
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
		return;

	// Get the indices of the object's position
	int xIndex = ((go->GetComponent<Transform>().GetPosition().x - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int yIndex = ((go->GetComponent<Transform>().GetPosition().y - (-ySize >> 1)) / (ySize / yNumOfGrid));
	int zIndex = ((go->GetComponent<Transform>().GetPosition().z - (-zSize >> 1)) / (zSize / zNumOfGrid));

	// Add them to each grid
	if (((xIndex >= 0) && (xIndex<xNumOfGrid)) &&
		((yIndex >= 0) && (yIndex<yNumOfGrid)) &&
		((zIndex >= 0) && (zIndex<zNumOfGrid)))
	{
		theGrid[xIndex * yIndex * zNumOfGrid + zIndex].Remove(theObject);
	}
	
}

/********************************************************************************
Calculate the squared distance from camera to a grid's centrepoint
********************************************************************************/
float SpatialPartition::CalculateDistanceSquare(const Vector3& theCameraPosition, const int xIndex, const int yIndex, const int zIndex)
{
	float xDistance = (xGridSize*xIndex + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition.x;
	float yDistance = (yGridSize*xIndex + (yGridSize >> 1) - (ySize >> 1)) - theCameraPosition.y;
	float zDistance = (zGridSize*zIndex + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition.z;

	return (float)(xDistance*xDistance + yDistance*yDistance +  zDistance*zDistance);
}

float SpatialPartition::CalculateDistance(const Vector3& theCameraPosition, const int xIndex, const int yIndex, const int zIndex)
{
	return sqrt(CalculateDistanceSquare(theCameraPosition, xIndex, yIndex, zIndex));
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
bool SpatialPartition::IsVisible(Vector3 theCameraPosition, Vector3 theCameraDirection, const int xIndex, const int yIndex, const int zIndex)
{
	float xDistance = (xGridSize*xIndex + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition.x;
	float yDistance = (yGridSize*xIndex + (yGridSize >> 1) - (ySize >> 1)) - theCameraPosition.y;
	float zDistance = (zGridSize*zIndex + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition.z;
	// If the camera is within the CGrid, then display by default
	// Otherwise, the entity may not get displayed.
	if (xDistance*xDistance + yDistance*yDistance + zDistance*zDistance < (xGridSize*xGridSize + yGridSize*yGridSize + zGridSize*zGridSize))
		return true;

	Vector3 gridCentre(xDistance, yDistance, zDistance);

	if (theCameraDirection.Dot(gridCentre) < 0)
		return false;

	return true;
}

/********************************************************************************
PrintSelf
********************************************************************************/
void SpatialPartition::PrintSelf() const
{
	cout << endl;
	cout << "**************** Start of CSpatialPartition::PrintSelf() ********************" << endl;
	cout << "xSize\t:\t" << xSize << "\tySize\t:\t" << ySize << "\tzSize\t:\t" << zSize << endl;
	cout << "xNumOfGrid\t:\t" << xNumOfGrid << "\tyNumOfGrid\t:\t" << yNumOfGrid << "\tzNumOfGrid\t:\t" << zNumOfGrid << endl;
	if (theGrid)
	{
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int k = 0; k < yNumOfGrid; k++)
			{
				for (int j = 0; j<zNumOfGrid; j++)
				{
					theGrid[i * k * zNumOfGrid + j].PrintSelf();
				}
			}
		}
		theGrid[xNumOfGrid * yNumOfGrid * zNumOfGrid].PrintSelf();
	}
	else
		cout << "theGrid : NULL" << endl;
	cout << "******* End of CSpatialPartition::PrintSelf() **********************************" << endl;
	cout << endl;
}
