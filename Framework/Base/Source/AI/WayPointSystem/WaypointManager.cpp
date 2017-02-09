//#include "WaypointManager.h"
//#include <limits>
//
//using std::numeric_limits;
//using std::cout;
//using std::endl;
//
//
//WaypointManager::WaypointManager()
//{
//	m_iID = -1;
//	listofWaypoints.clear();
//}
//
//WaypointManager::~WaypointManager()
//{
//	//if listofwaypoints is not empty, then proceed to delete
//	if (listofWaypoints.size() != 0)
//	{
//		//iterate through all the waypoints
//		//destroy the children first
//		vector<Waypoint*>::iterator it;
//		for (it = listofWaypoints.begin(); it != listofWaypoints.end(); ++it)
//		{
//			delete *it;
//		}
//	}
//	//we delete the waypoints here
//	listofWaypoints.clear();
//}
//
////add waypoint
//int WaypointManager::AddWaypoint(Vector3 position)
//{
//	m_iID++;
//	Waypoint* aNewWaypoint = new Waypoint();
//	aNewWaypoint->SetID(m_iID);
//	aNewWaypoint->SetPosition(position);
//	listofWaypoints.push_back(aNewWaypoint);
//	return m_iID;
//}
//
////add waypoint based on its m_iID
//int WaypointManager::AddWaypoint(const int m_iID, Vector3 position)
//{
//	if (listofWaypoints.size() != 0)
//	{
//		for (unsigned int i = 0; i < listofWaypoints.size(); ++i)
//		{
//			Waypoint* aWaypoint = listofWaypoints[i];
//
//			if (aWaypoint->GetID() == m_iID)
//			{
//				(this->m_iID)++;
//				Waypoint* aNewWaypoint = new Waypoint();
//				aNewWaypoint->SetID(this->m_iID);
//				aNewWaypoint->SetPosition(position);
//				listofWaypoints.push_back(aNewWaypoint);
//
//				//add the current waypoint as a related waypoint to the new waypoint
//				aNewWaypoint->AddRelatedWaypoint(aWaypoint);
//				//add this new waypoint as a related waypoint to the current waypoint
//				aWaypoint->AddRelatedWaypoint(aNewWaypoint);
//
//				return this->m_iID;
//			}
//		}
//		return -1;
//	}
//}
//
//bool WaypointManager::RemoveWaypoint(const int m_iID)
//{
//	bool result = false;
//
//	if (listofWaypoints.size() != 0)
//	{
//		std::vector<Waypoint*>::iterator it = listofWaypoints.begin();
//		while (it != listofWaypoints.end())
//		{
//			if ((*it)->GetID() == m_iID)
//			{
//				//remove all references to this waypoint from other waypoints
//				RemoveRelatedWaypoint(m_iID);
//
//				//remove this waypoint from the waypoint manager
//				delete *it;
//				it = listofWaypoints.erase(it);
//				result = true;
//				break;
//			}
//			if (it != listofWaypoints.end())
//				it++;
//		}
//	}
//	return result;
//}
//
//Waypoint* WaypointManager::GetWaypoint(const int m_iID)
//{
//	//if waypoints is not empty, then search
//	if (listofWaypoints.size() != 0)
//	{
//		std::vector<Waypoint*>::iterator it = listofWaypoints.begin();
//		while (it != listofWaypoints.end())
//		{
//			if ((*it)->GetID() == m_iID)
//			{
//				return (Waypoint*)(*it);
//			}
//			it++;
//		}
//	}
//	return NULL;
//}
//
////get nearest waypoint amongst related waypoints
//Waypoint* WaypointManager::GetNearestWaypoint(Vector3 aPosition)
//{
//	Waypoint* theNearestWaypoint = NULL;
//	float m_fDistance = numeric_limits<float>::max();
//
//	//if waypoints has related waypoints, then we proceed to search
//	if (listofWaypoints.size() > 0)
//	{
//		//iterate through all the waypoints
//		std::vector<Waypoint*>::iterator it = listofWaypoints.begin();
//
//		while (it != listofWaypoints.end())
//		{
//			Vector3 aRelatedWaypoint = (*it)->GetPosition();
//
//			float xDistance = aPosition.x - aRelatedWaypoint.x;
//			float zDistance = aPosition.z - aRelatedWaypoint.z; //
//			float distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
//			if (m_fDistance > distanceSquared)
//			{
//				m_fDistance = distanceSquared;
//				theNearestWaypoint = (Waypoint*)(*it);
//			}
//			it++;
//		}
//	}
//	return theNearestWaypoint;
//}
//
////get the number of related waypoints
//int WaypointManager::GetNumberOfWaypoints(void) const
//{
//	return listofWaypoints.size();
//}
//
////printself
//void WaypointManager::PrintSelf(void)
//{
//	cout << "==========================================" << endl;
//	cout << "WaypointManager::PrintSelf" << endl;
//	//if waypoints has related Waypoints, then we proceed to search
//	if (listofWaypoints.size() != 0)
//	{
//		vector<Waypoint*>::iterator it = listofWaypoints.begin();
//		while (it != listofWaypoints.end())
//		{
//			(*it)->PrintSelf();
//			it++;
//		}
//	}
//	cout << "========================================" << endl;
//}
//
//void WaypointManager::RemoveRelatedWaypoint(const int m_iID)
//{
//	//iterate through all the waypoints
//	std::vector<Waypoint*>::iterator it = listofWaypoints.begin();
//	while (it != listofWaypoints.end())
//	{
//		(*it)->RemoveRelatedWaypoint(m_iID);
//		it++;
//	}
//}