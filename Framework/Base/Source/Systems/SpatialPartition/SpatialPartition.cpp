#include "SpatialPartition.h"
#include "../../Graphics/GraphicsManager.h"

SpatialPartition::SpatialPartition()
	: xSize(0)
	, zSize(0)
	, xGridSize(0)
	, zGridSize(0)
	, xNumOfGrid(0)
	, zNumOfGrid(0)
	, yOffset(0.f)
{
	theGrid = new Grid();
}

SpatialPartition::~SpatialPartition()
{
	delete[] theGrid;
}

//initialise the size of the spatial partition and the number of partitions
bool SpatialPartition::Init(const int xGridSize, const int zGridSize,
	const int xNumOfGrid, const int zNumOfGrid,
	const float yOffset)
{
	if ((xGridSize>0) && (zGridSize>0) && (xNumOfGrid>0) && (zNumOfGrid>0))
	{
		vector<GameObjectID> ids;
		for (int i = 0; i < this->xNumOfGrid; i++)
		{
			for (int j = 0; j < this->zNumOfGrid; j++)
			{
				vector<GameObjectID> tempids = theGrid[i*this->zNumOfGrid + j].GetListOfObject();
				ids.insert(std::end(ids), std::begin(tempids), std::end(tempids));
			}
		}
		vector<GameObjectID> tempids = theGrid[this->xNumOfGrid * this->zNumOfGrid].GetListOfObject();
		ids.insert(std::end(ids), std::begin(tempids), std::end(tempids));
		if (this->xNumOfGrid* this->zNumOfGrid > 0)
			delete[] theGrid;
		else
			delete theGrid;

		this->xNumOfGrid = xNumOfGrid;
		this->zNumOfGrid = zNumOfGrid;
		this->xGridSize = xGridSize;
		this->zGridSize = zGridSize;
		this->xSize = xGridSize * xNumOfGrid;
		this->zSize = zGridSize * zNumOfGrid;
		this->yOffset = yOffset;

		// Create an array of grids
		theGrid = new Grid[xNumOfGrid * zNumOfGrid + 1];

		// Initialise the array of grids
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i*zNumOfGrid + j].Init(i, j, xGridSize, zGridSize, (xSize >> 1), (zSize >> 1));
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
	for (int i = 0; i< xNumOfGrid; i++)
	{
		for (int j = 0; j < zNumOfGrid; j++)
		{
			theGrid[i * zNumOfGrid + j].Update(&migrationList);

			/////////////////THIS CHECK SHOULD BE DONE IN RENDERSYSTEM////////////////////
			//// Check visibility
			//if (IsVisible(theCamera->GetCameraPos(),theCamera->GetCameraTarget() - theCamera->GetCameraPos(), i, j))
			//{
			//	// Calculate LOD for this CGrid
			//	float distance = CalculateDistanceSquare(&(theCamera->GetCameraPos()), i, j);
			//	if (distance < LevelOfDetails_Distances[0])
			//	{
			//		theGrid[i*zNumOfGrid + j].SetDetailLevel(LODMeshHolder::HIGH_DETAILS);
			//	}
			//	else if (distance < LevelOfDetails_Distances[1])
			//	{
			//		theGrid[i*zNumOfGrid + j].SetDetailLevel(LODMeshHolder::MID_DETAILS);
			//	}
			//	else
			//	{
			//		theGrid[i*zNumOfGrid + j].SetDetailLevel(LODMeshHolder::LOW_DETAILS);
			//	}
			//}
			//else
			//	theGrid[i*zNumOfGrid + j].SetDetailLevel(LODMeshHolder::NO_DETAILS);
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
Get zNumOfGrid
********************************************************************************/
int SpatialPartition::GetzNumOfGrid() const
{
	return zNumOfGrid;
}

/********************************************************************************
Get a particular grid
********************************************************************************/
Grid SpatialPartition::GetGrid(const int xIndex, const int yIndex) const
{
	return theGrid[xIndex * zNumOfGrid + yIndex];
}

/********************************************************************************
Get vector of objects from this Spatial Partition
********************************************************************************/
vector<GameObjectID> SpatialPartition::GetObjects(Vector3 position, const float radius)
{
	// Get the indices of the object's position
	int xIndex = (((int)position.x - (-xSize >> 1)) / (xSize / xNumOfGrid));
	int zIndex = (((int)position.z - (-zSize >> 1)) / (zSize / zNumOfGrid));

	return theGrid[xIndex*zNumOfGrid + zIndex].GetListOfObject();
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
	int zIndex = -1;
	if (xNumOfGrid != 0 && zNumOfGrid != 0)
	{
		xIndex = ((go->GetComponent<Transform>().GetPosition().x - (-xSize >> 1)) / (xSize / xNumOfGrid));
		zIndex = ((go->GetComponent<Transform>().GetPosition().z - (-zSize >> 1)) / (zSize / zNumOfGrid));
	}
	
	// Add them to each grid
	if (((xIndex >= 0) && (xIndex<xNumOfGrid)) && ((zIndex >= 0) && (zIndex<zNumOfGrid)))
	{
		theGrid[xIndex * zNumOfGrid + zIndex].Add(theObject);
	}
	else
	{
		//Check for None initialized Spatial Partition
		theGrid[zNumOfGrid * xNumOfGrid].Add(theObject);
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
	int zIndex = ((go->GetComponent<Transform>().GetPosition().z - (-zSize >> 1)) / (zSize / zNumOfGrid));

	// Add them to each grid
	if (((xIndex >= 0) && (xIndex<xNumOfGrid)) && ((zIndex >= 0) && (zIndex<zNumOfGrid)))
	{
		theGrid[xIndex * zNumOfGrid + zIndex].Remove(theObject);
	}
	
}

/********************************************************************************
Calculate the squared distance from camera to a grid's centrepoint
********************************************************************************/
float SpatialPartition::CalculateDistanceSquare(const Vector3& theCameraPosition, const int xIndex, const int zIndex)
{
	float xDistance = (xGridSize*xIndex + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition.x;
	float zDistance = (zGridSize*zIndex + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition.z;

	return (float)(xDistance*xDistance + zDistance*zDistance);
}

float SpatialPartition::CalculateDistance(const Vector3& theCameraPosition, const int xIndex, const int zIndex)
{
	return sqrt(CalculateDistanceSquare(theCameraPosition, xIndex, zIndex));
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
bool SpatialPartition::IsVisible(Vector3 theCameraPosition, Vector3 theCameraDirection, const int xIndex, const int zIndex)
{
	float xDistance = (xGridSize*xIndex + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition.x;
	float zDistance = (zGridSize*zIndex + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition.z;
	// If the camera is within the CGrid, then display by default
	// Otherwise, the entity may not get displayed.
	if (xDistance*xDistance + zDistance*zDistance < (xGridSize*xGridSize + zGridSize*zGridSize))
		return true;

	Vector3 gridCentre(xDistance, 0, zDistance);

	if (theCameraDirection.Dot(gridCentre) < 0)
		return false;

	return true;
}

/********************************************************************************
PrintSelf
********************************************************************************/
void SpatialPartition::PrintSelf() const
{
	cout << "******* Start of CSpatialPartition::PrintSelf() **********************************" << endl;
	cout << "xSize\t:\t" << xSize << "\tzSize\t:\t" << zSize << endl;
	cout << "xNumOfGrid\t:\t" << xNumOfGrid << "\tzNumOfGrid\t:\t" << zNumOfGrid << endl;
	if (theGrid)
	{
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i = 0; i<xNumOfGrid; i++)
		{
			for (int j = 0; j<zNumOfGrid; j++)
			{
				theGrid[i*zNumOfGrid + j].PrintSelf();
			}
		}
		theGrid[xNumOfGrid *zNumOfGrid].PrintSelf();
	}
	else
		cout << "theGrid : NULL" << endl;
	cout << "******* End of CSpatialPartition::PrintSelf() **********************************" << endl;
}
