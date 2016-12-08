#ifndef SPSYSTEM_H
#define SPSYSTEM_H

#include "SingletonTemplate.h"
#include "SpatialPartition.h"
#include <map>
#include <string>
using std::map;
using std::string;

class SpatialPartitionSystem : public Singleton < SpatialPartitionSystem >
{
	friend Singleton < SpatialPartitionSystem > ;
public:

	void Update(const string& spaceName);

	SpatialPartition* CreateSpatialPartition(const string& spaceName);
	bool DeleteSpatialPartition(const string& spaceName);
	SpatialPartition* GetSpatialPartition(const string& spaceName);
	bool HasSpatialPartition(const string& spaceName);
	void DeleteAllPartitions();

private:
	SpatialPartitionSystem();
	virtual ~SpatialPartitionSystem();
	map < string , SpatialPartition* > spMap;

};











#endif