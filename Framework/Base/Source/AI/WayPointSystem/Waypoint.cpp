//#include "Waypoint.h"
//#include <limits>
//
//using std::numeric_limits;
//using std::cout;
//using std::endl;
//
//Waypoint::Waypoint()
//	: position(0, 0, 0)
//	, m_iID(-1)
//{
//	relatedWaypoints.clear();
//}
//
//Waypoint::~Waypoint()
//{}
//
//void Waypoint::SetPosition(Vector3 position)
//{
//	this->position = position;
//}
//
//Vector3 Waypoint::GetPosition() const
//{
//	return position;
//}
//
//void Waypoint::SetID(const int m_iID)
//{
//	this->m_iID = m_iID;
//}
//
//int Waypoint::GetID() const
//{
//	return m_iID;
//}
//
//void Waypoint::AddRelatedWaypoint(Waypoint * relatedWaypoint)
//{
//	relatedWaypoints.push_back(relatedWaypoint);
//}
//
//bool Waypoint::RemoveRelatedWaypoint(const int m_iID)
//{
//	if (relatedWaypoints.size() > 0)
//	{
//		vector<Waypoint*>::iterator iter = relatedWaypoints.begin();
//
//		if ((*iter)->GetID() == m_iID)
//		{
//			iter = relatedWaypoints.erase(iter);
//			return true;
//		}
//		iter++;
//	}
//	return false;
//}
//
//Waypoint* Waypoint::GetNearestWaypoint()
//{
//	Waypoint* nearestWaypoint = nullptr;
//	float shortestDistance = numeric_limits<float>::max();
//
//	if (relatedWaypoints.size() > 0)
//	{
//		vector<Waypoint*>::iterator iter = relatedWaypoints.begin();
//		while (iter != relatedWaypoints.end())
//		{
//			float distanceSquared = ((*iter)->GetPosition() - position).LengthSquared();
//
//			if (distanceSquared < shortestDistance)
//			{
//				shortestDistance = distanceSquared;
//				nearestWaypoint = (Waypoint*)(*iter);
//			}
//			++iter;
//		}
//	}
//	return nearestWaypoint;
//}
//
//int Waypoint::GetNumberOfWaypoints() const 
//{
//	return relatedWaypoints.size();
//}
//
////Debug code
//void Waypoint::PrintSelf()
//{
//	cout << "waypoint ID :" << m_iID << endl;
//	cout << "position : " << position << endl;
//	cout << "related waypoints" << endl;
//	if (relatedWaypoints.size() > 0)
//	{
//		vector<Waypoint*>::iterator iter = relatedWaypoints.begin();
//		while (iter != relatedWaypoints.end())
//		{
//			cout << (*iter)->GetID() << endl;
//			iter++;
//		}
//	}
//	else
//		cout << "NIL" << endl;
//}