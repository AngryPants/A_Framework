//#ifndef WAYPOINT_MANAGER_H
//#define WAYPOINT_MANAGER_H
//
//#include "SingletonTemplate.h"
//#include "Waypoint.h"
//#include "Vector3.h"
//#include <vector>
//using std::vector;
//
//class WaypointManager :public Singleton<WaypointManager>
//{
//	friend Singleton < WaypointManager > ;
//protected:
//	WaypointManager();
//	virtual ~WaypointManager();
//
//	int m_iID;
//	vector<Waypoint*> listofWaypoints;
//
//	//remove related waypoint
//	void RemoveRelatedWaypoint(const int m_iID);
//
//public:
//	//add waypoint
//	int AddWaypoint(Vector3 position);
//	//add waypoint based on its m_iID
//	int AddWaypoint(const int m_iID, Vector3 position);
//	//remove waypoint based on its m_iID
//	bool RemoveWaypoint(const int m_iID);
//
//	//get a waypoint based on its ID
//	Waypoint* GetWaypoint(const int m_iID);
//	//get nearest waypoint amongst related waypoints
//	Waypoint* GetNearestWaypoint(Vector3 aPositon);
//
//	//get the number of related waypoints
//	int GetNumberOfWaypoints(void) const;
//
//	//Printself
//	void PrintSelf(void);
//};
//
//#endif