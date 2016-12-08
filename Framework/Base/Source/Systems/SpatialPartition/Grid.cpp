#include "Grid.h"
#include "../../Mesh/MeshBuilder.h"
#include "../../Graphics/RenderHelper.h"

Grid::Grid()
	: index(Vector3(-1, -1, -1))
	, size(Vector3(-1, -1, -1))
	, offset(Vector3(-1, -1, -1))
	, min(Vector3(-1, -1, -1))
	, max(Vector3(-1, -1, -1))
	, listOfObjects(NULL) 
{
}

Grid::~Grid()
{
	RemoveAll();
}

void Grid::Init(const int xIndex, const int zIndex,
	const int xGridSize, const int zGridSize,
	const float xOffset, const float zOffset)
{
	index.Set(xIndex, 0, zIndex);
	size.Set(xGridSize, 0, zGridSize);
	offset.Set(xOffset, 0, zOffset);
	min.Set(index.x * size.x - offset.x, 0.0f, index.z * size.z - offset.z);
	max.Set(index.x * size.x - offset.x + xGridSize, 0.0f, index.z * size.z - offset.z + zGridSize);
}

// Update the grid
void Grid::Update(vector<GameObjectID>* migrationList)
{
	// Check each object to see if they are no longer in this grid
	std::vector<GameObjectID>::iterator it;
	it = listOfObjects.begin();
	while (it != listOfObjects.end())
	{
		GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(*it);
		
		if (go == nullptr)
			continue;
		
		Vector3 position = go->GetComponent<Transform>().GetPosition();

		if (((min.x <= position.x) && (position.x <= max.x)) &&
			((min.z <= position.z) && (position.z <= max.z)))
		{
			// Move on otherwise
			++it;
		}
		else
		{
			migrationList->push_back(*it);

			// Remove from this Grid
			it = listOfObjects.erase(it);
		}
	}
}

// Add a new object to this grid. returns false if already have that object in the list
bool Grid::Add(GameObjectID theObject)
{
	if (!IsHere(theObject))
	{
		listOfObjects.push_back(theObject);
		return true;
	}
	return false;
}

// Remove but not delete all objects from this grid
void Grid::RemoveAll()
{
	for (size_t i = 0; i < listOfObjects.size(); i++)
	{
		// Do not delete the objects as they are stored in GameObjectManager and will be deleted there.
		listOfObjects[i] = NULL;
	}
	listOfObjects.clear();
}

// Remove but not delete an object from this grid
bool Grid::Remove(GameObjectID theObject)
{
	// Clean up entities that are done
	std::vector<GameObjectID>::iterator it, end;
	it = listOfObjects.begin();
	end = listOfObjects.end();
	while (it != end)
	{
		if ((*it) == theObject)
		{
			it = listOfObjects.erase(it);
			return true;
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
	return false;
}

// Check if an object is in this grid
bool Grid::IsHere(GameObjectID theObject) const
{
	for (size_t i = 0; i < listOfObjects.size(); ++i)
	{
		if (listOfObjects[i] == theObject)
			return true;
	}
	return false;
}

// Get list of objects in this grid
vector<GameObjectID> Grid::GetListOfObject()
{
	return listOfObjects;
}

void Grid::PrintSelf()
{
	if (listOfObjects.size() > 0)
	{
		cout << "Grid::PrintSelf()" << endl;
		cout << "\tIndex\t:\t" << index << "\t\tOffset\t:\t" << offset << endl;
		cout << "\tMin\t:\t" << min << "\tMax\t:\t" << max << endl;
		cout << "\t------------------------------------------------------------------------" << endl;

		for (int i = 0; i < listOfObjects.size(); ++i)
		{
			GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(listOfObjects[i]);

			if (go == nullptr)
				continue;

			cout << "\t" << i << "\t:\t" << go->GetComponent<Transform>().GetPosition() << endl;
		}

		cout << "\t------------------------------------------------------------------------" << endl;
		cout << "********************************************************************************" << endl;
	}
}