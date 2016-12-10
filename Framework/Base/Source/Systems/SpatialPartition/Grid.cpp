#include "Grid.h"
#include "../../Mesh/MeshBuilder.h"
#include "../../Graphics/RenderHelper.h"

Grid::Grid()
	: index(0, 0, 0)
	, gridSize(0, 0, 0)
	//, offset(0, 0, 0)
	, position(0, 0, 0)
	, min(0, 0, 0)
	, max(0, 0, 0)
{
}

Grid::~Grid()
{
	RemoveAll();
}

void Grid::Set(const GridIndex _index,
			 const int _xGridSize, const int _yGridSize, const int _zGridSize,
			 const int _xOffset, const int _yOffset, const int _zOffset)
{
	this->index = _index;
	this->gridSize.Set(_xGridSize, _yGridSize, _zGridSize);
	//this->offset.Set(_offsetX, _offsetY, _offsetZ);

	this->min.Set(index.x * gridSize.x + _xOffset, index.y * gridSize.y + _yOffset, index.z * gridSize.z + _zOffset);
	this->max = min + gridSize;
	this->position = min + (gridSize * 0.5f);
	
}

GridIndex Grid::GetIndex() const {
	return index;
}	

Vector3 Grid::GetPosition() const {
	return position;
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
		{
			continue;
		}			
		
		Vector3 position = go->GetComponent<Transform>().GetPosition();

		if (((min.x <= position.x) && (position.x <= max.x)) &&
			((min.y <= position.y) && (position.y <= max.y)) &&
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
		{
			return true;
		}			
	}
	return false;
}

// Get list of objects in this grid
vector<GameObjectID> Grid::GetListOfObject()
{
	return listOfObjects;
}

int Grid::GetNumObjects() const {
	return listOfObjects.size();
}

void Grid::PrintSelf()
{
	if (listOfObjects.size() > 0)
	{
		cout << "Grid::PrintSelf()" << endl;
		cout << "\tIndex\t:\t" << index << "\t\tPosition\t:\t" << position << endl;
		cout << "\tMin\t:\t" << min << "\tMax\t:\t" << max << endl;
		cout << "\t------------------------------------------------------------------------" << endl;

		for (int i = 0; i < listOfObjects.size(); ++i)
		{
			GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(listOfObjects[i]);

			if (go == nullptr)
			{
				continue;
			}		

			cout << "\t" << i << " (" << go->GetID() << ")" << go->GetName() << "\t:\t" << go->GetComponent<Transform>().GetPosition() << endl;
		}

		cout << "\t------------------------------------------------------------------------" << endl;
		cout << "********************************************************************************" << endl;
	}
}