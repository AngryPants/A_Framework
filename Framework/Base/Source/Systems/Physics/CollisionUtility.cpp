#include "CollisionUtility.h"

CollisionUtility::CollisionUtility()
{

}

CollisionUtility::~CollisionUtility()
{

}

bool CollisionUtility::CheckAccurateSphereSphereCollision(Vector3 positionA, Vector3& velocityA, float radiusA, Vector3 positionB, Vector3 velocityB, float radiusB)
{
	// Early Escape test: if the length of the movevec is less
	// than distance between the centers of these circles minus
	// their radii, there's no way they can hit.
	double dist = (positionB - positionA).Length();//B.center.distance(A.center);
	double sumRadii = (radiusA + radiusB);

	if (dist <= sumRadii) {

		return true;

	}

	dist -= sumRadii;

	Vector3 movevec;
	movevec = velocityB - velocityA;

	if (movevec.Length() < dist) {

		return false;

	}

	// Normalize the movevec
	Vector3 N = movevec; //This crashes when trying to normalise.
	N.Normalize();

	// Find C, the vector from the center of the moving
	// circle A to the center of B
	Vector3 C = positionB - positionA;//.center.minus(A.center);

	// D = N . C = ||C|| * cos(angle between N and C)
	double D = N.Dot(C);//N.dot(C);

	// Another early escape: Make sure that A is moving
	// towards B! If the dot product between the movevec and
	// B.center - A.center is less that or equal to 0,
	// A isn't isn't moving towards B
	if (D <= 0){
		return false;
	}

	// Find the length of the vector C
	double lengthC = C.Length();

	double F = (lengthC * lengthC) - (D * D);

	// Escape test: if the closest that A will get to B
	// is more than the sum of their radii, there's no
	// way they are going collide
	double sumRadiiSquared = sumRadii * sumRadii;
	if (F >= sumRadiiSquared){
		return false;
	}

	// We now have F and sumRadii, two sides of a right triangle.
	// Use these to find the third side, sqrt(T)
	double T = sumRadiiSquared - F;

	// If there is no such right triangle with sides length of
	// sumRadii and sqrt(f), T will probably be less than 0.
	// Better to check now than perform a square root of a
	// negative number.
	if (T < 0){
		return false;
	}

	// Therefore the distance the circle has to travel along
	// movevec is D - sqrt(T)
	double distance = D - sqrt(T);

	// Get the magnitude of the movement vector
	double mag = movevec.Length();

	// Finally, make sure that the distance A has to move
	// to touch B is not greater than the magnitude of the
	// movement vector.
	if (mag < distance){
		return false;
	}

	// Set the length of the movevec so that the circles will just
	// touch
	movevec.Normalize();
	velocityA = movevec*distance;

	return true;
}


//RayCasting (infiniteLength)
bool CollisionUtility::CheckCollisionRayBox(Vector3 lineOrigin, Vector3 lineForward, Vector3 positionBox, Vector3 sizeBox, float& distance)
{
	///raycast below///
	Vector3 minPosBox, maxPosBox;

	minPosBox = Vector3(positionBox.x - sizeBox.x, positionBox.y - sizeBox.y, positionBox.z - sizeBox.z);
	maxPosBox = Vector3(positionBox.x + sizeBox.x, positionBox.y + sizeBox.y, positionBox.z + sizeBox.z);

	Vector3 dirfrac;
	// r.dir is unit direction vector of ray
	dirfrac.x = 1.0f / lineForward.x;
	dirfrac.y = 1.0f / lineForward.y;
	dirfrac.z = 1.0f / lineForward.z;
	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (minPosBox.x - lineOrigin.x) * dirfrac.x;
	float t2 = (maxPosBox.x - lineOrigin.x) * dirfrac.x;
	float t3 = (minPosBox.y - lineOrigin.y) * dirfrac.y;
	float t4 = (maxPosBox.y - lineOrigin.y) * dirfrac.y;
	float t5 = (minPosBox.z - lineOrigin.z) * dirfrac.z;
	float t6 = (maxPosBox.z - lineOrigin.z) * dirfrac.z;

	float tmin = Math::Max(Math::Max(Math::Min(t1, t2), Math::Min(t3, t4)), Math::Min(t5, t6));
	float tmax = Math::Min(Math::Min(Math::Min(t1, t2), Math::Max(t3, t4)), Math::Max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0)
	{
		//t is distance
		distance = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		//t is distance
		distance = tmax;
		return false;
	}

	distance = tmin;
	return true;

}

bool CollisionUtility::CheckCollisionRaySphere(Vector3 lineOrigin, Vector3 lineDir, Vector3 positionSphere, float sphereRadius, Vector3& collisionPoint, Vector3& collisionNormal)
{
	Vector3 sphereDir = positionSphere - lineOrigin;
	Vector3 projection = sphereDir.Projection(lineDir);

	Vector3 closestPt = lineOrigin + projection;

	float distSquared = (positionSphere - closestPt).LengthSquared();

	if (distSquared > sphereRadius*sphereRadius)
	{
		return false;
	}
	 
	float m = sqrt( distSquared + sphereRadius*sphereRadius);

	if (!lineDir.IsZero())
	{
		collisionPoint = closestPt - m* (lineDir.Normalized());
	}
	else
	{
		collisionPoint = closestPt;
	}

	try {
		collisionNormal = (collisionPoint - positionSphere).Normalize();
	}
	catch (DivideByZero e) {
		collisionNormal.Set(0, 0, 1);
	}

	return true;
}

bool CollisionUtility::CheckCollisionRayPlane(const Vector3& lineOrigin, const Vector3& dirVec, const Vector3& planeCenter, const Vector3& planeNormal, float& timeTaken)
{
	// assuming vectors are all normalized
	float denom = planeNormal.Dot(dirVec);
	if (fabs(denom) > Math::EPSILON) {
		Vector3 distToPlane = planeCenter - lineOrigin;
		timeTaken = distToPlane.Dot(planeNormal) / denom;
		return (timeTaken >= 0);
	}

	return false;
}

bool CollisionUtility::CheckCollisionRayDisk(const Vector3 &normal, const Vector3 &planeCenter, const float &radius, const Vector3 &lineOrigin, const Vector3 &lineDir)
{
	float t = 0;
	if (CheckCollisionRayPlane(lineOrigin, lineDir, planeCenter,normal,t)) 
	{
		Vector3 p = lineOrigin + lineDir * t;
		Vector3 v = p - lineOrigin;
		float d2 = v.Dot(v);
		return (d2 <= radius* radius);
		// or you can use the following optimisation (and precompute radius^2)
		// return d2 <= radius2; // where radius2 = radius * radius
	}

	return false;
}

//Line Collision (finite length)
bool CollisionUtility::CheckCollisionLineSphere(Vector3 lineOrigin, Vector3 lineDir, float lineLength, Vector3 spherePosition, float radius)
{
	if (lineDir.IsZero()) {
		return ((lineOrigin - spherePosition).LengthSquared() < radius * radius);
	}
	
	lineDir.Normalize();

	Vector3 closestPoint = lineOrigin + lineDir.Projection(spherePosition - lineOrigin);
	if ((closestPoint - lineOrigin).LengthSquared() < lineLength * lineLength) {
		return (closestPoint - spherePosition).LengthSquared() < radius * radius;
	}
	return false;
}

//Line AABB, pass in Hit to find out where it is hit at hit ( collisionPoint )
bool inline CollisionUtility::GetIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f) 
		return 0;
	if (fDst1 == fDst2) 
		return 0;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return 1;
}

bool inline CollisionUtility::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return 1;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return 1;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
	return 0;
}

bool CollisionUtility::CheckCollisionLineBox(Vector3 minPosBox, Vector3 maxPosBox, Vector3 lineOrigin, Vector3 lineEnd, Vector3 &Hit)
{
	// returns true if line (L1, L2) intersects with the box (B1, B2)
	// returns intersection point in Hit
	if (lineEnd.x < minPosBox.x && lineOrigin.x < minPosBox.x) return false;
	if (lineEnd.x > maxPosBox.x && lineOrigin.x > maxPosBox.x) return false;
	if (lineEnd.y < minPosBox.y && lineOrigin.y < minPosBox.y) return false;
	if (lineEnd.y > maxPosBox.y && lineOrigin.y > maxPosBox.y) return false;
	if (lineEnd.z < minPosBox.z && lineOrigin.z < minPosBox.z) return false;
	if (lineEnd.z > maxPosBox.z && lineOrigin.z > maxPosBox.z) return false;
	if (lineOrigin.x > minPosBox.x && lineOrigin.x < maxPosBox.x &&
		lineOrigin.y > minPosBox.y && lineOrigin.y < maxPosBox.y &&
		lineOrigin.z > minPosBox.z && lineOrigin.z < maxPosBox.z)
	{
		Hit = lineOrigin;
		return true;
	}
	if ((GetIntersection(lineOrigin.x - minPosBox.x, lineEnd.x - minPosBox.x, lineOrigin, lineEnd, Hit) && InBox(Hit, minPosBox, maxPosBox, 1))
		|| (GetIntersection(lineOrigin.y - minPosBox.y, lineEnd.y - minPosBox.y, lineOrigin, lineEnd, Hit) && InBox(Hit, minPosBox, maxPosBox, 2))
		|| (GetIntersection(lineOrigin.z - minPosBox.z, lineEnd.z - minPosBox.z, lineOrigin, lineEnd, Hit) && InBox(Hit, minPosBox, maxPosBox, 3))
		|| (GetIntersection(lineOrigin.x - maxPosBox.x, lineEnd.x - maxPosBox.x, lineOrigin, lineEnd, Hit) && InBox(Hit, minPosBox, maxPosBox, 1))
		|| (GetIntersection(lineOrigin.y - maxPosBox.y, lineEnd.y - maxPosBox.y, lineOrigin, lineEnd, Hit) && InBox(Hit, minPosBox, maxPosBox, 2))
		|| (GetIntersection(lineOrigin.z - maxPosBox.z, lineEnd.z - maxPosBox.z, lineOrigin, lineEnd, Hit) && InBox(Hit, minPosBox, maxPosBox, 3)))
		return true;

	return false;
}

bool CollisionUtility::CheckCollisionLineAABB(Vector3 lineOrigin, Vector3 lineForward, float lineLength, Vector3 positionBox, Vector3 sizeBox, Vector3& hit)
{
	Vector3 minPosBox, maxPosBox;

	minPosBox = Vector3(positionBox.x - sizeBox.x, positionBox.y - sizeBox.y, positionBox.z - sizeBox.z);
	maxPosBox = Vector3(positionBox.x + sizeBox.x, positionBox.y + sizeBox.y, positionBox.z + sizeBox.z);

	Vector3 lineEnd = lineOrigin + lineForward * lineLength;
	return CheckCollisionLineBox(minPosBox, maxPosBox, lineOrigin, lineEnd, hit);
}

//Sphere collision
bool CollisionUtility::CheckCollisionSpherePoint(Vector3 positionA, float radiusA, Vector3 positionB)
{
	float distBetweenVectors = (positionA - positionB).LengthSquared();
	return (distBetweenVectors < radiusA*radiusA);
}

//Cheap Empirical
bool CollisionUtility::CheckCollisionSphereSphere_Empirical(Vector3 positionA, float radiusA, Vector3 positionB, float radiusB)
{
	float distBetweenVectors = (positionA - positionB).LengthSquared();
	return (distBetweenVectors < radiusA*radiusA + radiusB*radiusB);
}

bool CollisionUtility::CheckCollisionSphereSphere_Mathematical(const Vector3& _positionA, const Vector3& _velocityA, float _radiusA,
															   const Vector3& _positionB, const Vector3& _velocityB, float _radiusB,
															   double _deltaTime,
															   Vector3& _collisionPoint, Vector3& _collisionNormal, float& _separation) {
	//The direction from A to B
	Vector3 relativeDisplacement = _positionB - _positionA;
	//Assume B is stationary
	Vector3 relativeVelocity = _velocityA - _velocityB;
	float combinedRadius = _radiusA + _radiusB;

	//Make sure A is moving to B
	if (relativeDisplacement.Dot(relativeVelocity) <= Math::EPSILON) {
		return false;
	}

	{
		//Check for time to collision
		float a = relativeVelocity.Dot(relativeVelocity); //Or we can use relativeVelocity.LengthSquared();
		float b = 2.0f * relativeDisplacement.Dot(relativeVelocity);
		float c = relativeDisplacement.LengthSquared() - combinedRadius * combinedRadius;

		/*
		Now we test the discriminant to see if there is a collision in the first place.
		Let the discriminant be d.
		If d < 0. There is no collision. If d >= 0, there is a collision.
		d = b^2 - 4ac
		*/

		float d = (b*b) - (4.0f*a*c);

		if (d < 0.0f) {
			return false; //There is no collision.
		}

		//Since there is collision, time to find the timeToCollision.

		//Since there are 2 possible answers, we take the smaller one first.
		float timeToCollision = (-b - sqrt(d)) / (2.0f * a);

		//We reject it if it is < 0. As time cannot be < 0, the smaller timing is impossible.
		if (timeToCollision < 0.0f) {
			timeToCollision = (-b + sqrt(d)) / (2.0f * a); //Then we take the second one.
		}
		//The reason we check the smaller answer instead of just taking the larger timing is that both could be positive,
		//and it means that the line collided first at the smaller timing.

		if (timeToCollision < _deltaTime) {
			return false;
		}

		//Now that we know that there's a collision, find additional info.
		_collisionPoint = _positionA + (timeToCollision * relativeVelocity);

		try {
			_collisionNormal = (_collisionPoint - _positionB).Normalize();
			_separation = combinedRadius;
		} catch (DivideByZero e) {
			_collisionNormal.Set(0, 0, 1);
			_separation = 0.0f;
		}
	}

	/*{
		//Now that we know that there's a collision, find additional info.
		Vector3 projection;
		if (relativeVelocity.LengthSquared() > Math::EPSILON) {
			projection = relativeDisplacement.Projection(relativeVelocity);
		}
		//Vector3 projection = relativeDisplacement.Projection(relativeVelocity);

		Vector3 closestPt = _positionA + projection;
		//Assume A is a line and add the radius to B.
		float distSquared = (_positionB - closestPt).LengthSquared();
		if (distSquared > combinedRadius * combinedRadius) {
			return false;
		}

		//How far the cloeset point is to the collisionPoint
		float distanceToCollisionPoint = sqrt(distSquared + combinedRadius * combinedRadius);
		if (!relativeDisplacement.IsZero()) {
			_collisionPoint = closestPt - distanceToCollisionPoint * (relativeDisplacement.Normalized());
		} else {
			_collisionPoint = closestPt;
		}

		try {
			_collisionNormal = (_collisionPoint - _positionB).Normalize();
			_separation = combinedRadius;
		} catch (DivideByZero e) {
			_collisionNormal.Set(0, 0, 1);
			_separation = 0.0f;
		}
	}*/

	return true;
}

bool CollisionUtility::CheckCollisionSphereAABBMinMax(Vector3 positionSphere, float radiusSphere, Vector3 minPosBox, Vector3 maxPosBox)
{
	Vector3 closestPoint;
	// get box closest point to sphere center by clamping
	closestPoint.x = Math::Max(minPosBox.x, Math::Min(positionSphere.x, maxPosBox.x));
	closestPoint.y = Math::Max(minPosBox.y, Math::Min(positionSphere.y, maxPosBox.y));
	closestPoint.z = Math::Max(minPosBox.z, Math::Min(positionSphere.z, maxPosBox.z));

	// this is the same as isPointInsideSphere
	float distance = (closestPoint - positionSphere).LengthSquared();

	return (distance < radiusSphere*radiusSphere);
}

bool CollisionUtility::CheckCollisionSphereAABB(Vector3 positionSphere, float radiusSphere, Vector3 positionBox, Vector3 sizeBox)
{
	Vector3 minPosBox, maxPosBox;

	minPosBox = Vector3(positionBox.x - sizeBox.x * 0.5f, positionBox.y - sizeBox.y * 0.5f, positionBox.z - sizeBox.z * 0.5f);
	maxPosBox = Vector3(positionBox.x + sizeBox.x * 0.5f, positionBox.y + sizeBox.y * 0.5f, positionBox.z + sizeBox.z * 0.5f);

	return CheckCollisionSphereAABBMinMax(positionSphere, radiusSphere, minPosBox, maxPosBox);
}

//AABB collision
bool CollisionUtility::CheckCollisionAABBMinMax(Vector3 minPosA, Vector3 maxPosA, Vector3 minPosB, Vector3 maxPosB)
{
	return (minPosA.x <= maxPosB.x && maxPosA.x >= minPosB.y) &&
		(minPosA.y <= maxPosB.y && maxPosA.y >= minPosB.y) &&
		(minPosA.z <= maxPosB.z && maxPosA.z >= minPosB.z);
}

bool CollisionUtility::CheckCollisionAABB(Vector3 positionA, Vector3 sizeA, Vector3 positionB, Vector3 sizeB)
{
	Vector3 minPosA, maxPosA, minPosB, maxPosB;

	minPosA = Vector3(positionA.x - sizeA.x * 0.5f, positionA.y - sizeA.y * 0.5f, positionA.z - sizeA.z * 0.5f);
	maxPosA = Vector3(positionA.x + sizeA.x * 0.5f, positionA.y + sizeA.y * 0.5f, positionA.z + sizeA.z * 0.5f);
	minPosB = Vector3(positionB.x - sizeB.x * 0.5f, positionB.y - sizeB.y * 0.5f, positionB.z - sizeB.z * 0.5f);
	maxPosB = Vector3(positionB.x + sizeB.x * 0.5f, positionB.y + sizeB.y * 0.5f, positionB.z + sizeB.z * 0.5f);

	return CheckCollisionAABBMinMax(minPosA, maxPosA, minPosB, maxPosB);
}

bool CollisionUtility::CheckCollisionAABBPointMinMax(Vector3 minPosA, Vector3 maxPosA, Vector3 positionB)
{
	return (minPosA.x <= positionB.x && maxPosA.x >= positionB.y) &&
		(minPosA.y <= positionB.y && maxPosA.y >= positionB.y) &&
		(minPosA.z <= positionB.z && maxPosA.z >= positionB.z);
}

bool CollisionUtility::CheckCollisionAABBPoint(Vector3 positionA, Vector3 sizeA, Vector3 positionB)
{
	Vector3 minPosA, maxPosA;

	minPosA = Vector3(positionA.x - sizeA.x, positionA.y - sizeA.y, positionA.z - sizeA.z);
	maxPosA = Vector3(positionA.x + sizeA.x, positionA.y + sizeA.y, positionA.z + sizeA.z);

	return CheckCollisionAABBPointMinMax(minPosA, maxPosA, positionB);
}
