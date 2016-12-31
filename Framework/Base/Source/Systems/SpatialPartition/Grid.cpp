#include "Grid.h"
#include "../../Mesh/MeshBuilder.h"
#include "../../Graphics/RenderHelper.h"
#include "../../GameObject/GameObject.h"

Grid::Grid()
	: index(-1)
	, position(0.0f, 0.0f, 0.0f)
	, min(0.0f, 0.0f, 0.0f)
	, max(0.0f, 0.0f, 0.0f)
{
}

Grid::~Grid() {
	RemoveAll();
}

void Grid::Set(int _index, Vector3 _position, Vector3 _size) {
	this->index = _index;
	this->position = _position;
	this->min = _position - (_size * 0.5f);
	this->max = _position + (_size * 0.5f);
}

GridIndex Grid::GetIndex() const {
	return index;
}	

Vector3 Grid::GetPosition() const {
	return position;
}

Vector3 Grid::GetMin() const {
	return min;
}

Vector3 Grid::GetMax() const {
	return max;
}

//Update the grid
/*void Grid::Update(vector<GameObjectID>* migrationList) {
	//Check if we are the extra grid.
	if (index.x < 0 || index.y < 0 || index.z < 0) {
		for (unsigned int i = 0; i < listOfObjects.size(); ++i) {
			migrationList->push_back(listOfObjects[i]);
		}
		listOfObjects.clear();
		return;
	}

	//Check each object to see if they are no longer in this grid
	std::vector<GameObjectID>::iterator iter = listOfObjects.begin();
	while (iter != listOfObjects.end()) {
		GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(*iter);
		Vector3 position = go->GetComponent<Transform>().GetPosition();
		if (go->useSpatialPartition && 
			min.x <= position.x && position.x <= max.x &&
			min.y <= position.y && position.y <= max.y &&
			min.z <= position.z && position.z <= max.z)
		{
			//Move on otherwise
			++iter;
		} else {
			migrationList->push_back(*iter);
			//Remove from this Grid
			iter = listOfObjects.erase(iter);
		}
	}
}*/



// Add a new object to this grid. returns false if already have that object in the list
bool Grid::Add(GameObjectID theObject) {
	goList.push_back(theObject);
	GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(theObject);
	go->SetGridIndex(index, {});
	return true;
}

// Remove but not delete all objects from this grid
void Grid::RemoveAll() {
	goList.clear();
}

// Remove but not delete an object from this grid
bool Grid::Remove(GameObjectID _gameObjectID) {
	GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(_gameObjectID);
	if (go->GetGridIndex() != index) {
		cout << "GridIndex mismatch!" << endl;
		cout << "_gameObjectID: " << _gameObjectID << endl;
		cout << "GridIndex: " << index << endl;
		cout << "GO GridIndex: " << go->GetGridIndex() << endl;
	}

	for (vector<GameObjectID>::iterator vecIter = goList.begin(); vecIter != goList.end(); ++vecIter) {
		if ((*vecIter) == _gameObjectID) {
			goList.erase(vecIter);
			return true;			
		}
	}

	return false;
}

// Check if an object is in this grid
bool Grid::IsHere(GameObjectID theObject) const {
	for (size_t i = 0; i < goList.size(); ++i) {
		if (goList[i] == theObject) {
			return true;
		}
	}
	return false;
}

bool Grid::WithinGridBoundaries(const Vector3& _position) const {
	return (_position.x >= min.x && _position.x <= max.x &&
			_position.y >= min.y && _position.y <= max.y &&
			_position.z >= min.z && _position.z <= max.z);
}

int Grid::GetNumObjects() const {
	return goList.size();
}

void Grid::PrintSelf() {
	if (!goList.empty()) {
		cout << "Grid::PrintSelf()" << endl;
		cout << "\tIndex\t:\t" << index << "\t\tPosition\t:\t" << position << endl;
		cout << "\tMin\t:\t" << min << "\tMax\t:\t" << max << endl;
		cout << "\t------------------------------------------------------------------------" << endl;

		for (int i = 0; i < goList.size(); ++i) {
			GameObject* go = GameObjectManager::GetInstance().GetGameObjectByID(goList[i]);
			if (go == nullptr) {
				continue;
			}
			cout << "\t" << i << ". [" << go->GetID() << "]" << go->GetName() << "\t:\t" << go->GetComponent<Transform>().GetPosition() << endl;
		}

		cout << "\t------------------------------------------------------------------------" << endl;
		cout << "********************************************************************************" << endl;
	}
}