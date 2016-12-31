#include "SPSystem.h"

SpatialPartitionSystem::SpatialPartitionSystem()
{
}

SpatialPartitionSystem::~SpatialPartitionSystem()
{
	DeleteAllPartitions();
}

void SpatialPartitionSystem::Update(const string& spaceName)
{
	SpatialPartition * sp = GetSpatialPartition(spaceName);
	if (sp != nullptr)
	{
		sp->Update();
	}		
}

SpatialPartition* SpatialPartitionSystem::CreateSpatialPartition(const string& spaceName)
{
	map<string, SpatialPartition*>::iterator mapIter = spMap.find(spaceName);
	if (mapIter != spMap.end())
	{
		return mapIter->second;
	}
	else
	{
		SpatialPartition* sp = new SpatialPartition(spaceName);
		spMap.insert(std::pair<string, SpatialPartition*>(spaceName, sp));
		return sp;
	}
}

bool SpatialPartitionSystem::DeleteSpatialPartition(const string& spaceName)
{
	map<string, SpatialPartition*>::iterator mapIter = spMap.find(spaceName);
	if (mapIter != spMap.end())
	{
		delete mapIter->second;
		spMap.erase(mapIter);
		return true;
	}

	return false;
}

SpatialPartition* SpatialPartitionSystem::GetSpatialPartition(const string& spaceName)
{
	map<string, SpatialPartition*>::iterator mapIter = spMap.find(spaceName);
	if (mapIter != spMap.end())
	{
		return mapIter->second;
	}
	return nullptr;
}

bool SpatialPartitionSystem::HasSpatialPartition(const string& spaceName)
{
	return spMap.count(spaceName) != 0;
}

void SpatialPartitionSystem::DeleteAllPartitions()
{
	for (map<string, SpatialPartition*>::iterator mapIter = spMap.begin(); mapIter != spMap.end(); mapIter++)
	{
		delete mapIter->second;
	}
	spMap.clear();
}