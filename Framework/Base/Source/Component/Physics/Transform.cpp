/*******************************************************************************/
/*!
\file Transform.cpp
\author Lim Ngian Xin Terry
\par email: lnxterry\@gmail.com

\brief
CPP file for Transform.
*/
/*******************************************************************************/

#include "Transform.h"
#include <iostream>
#include "../../GameObject/GameObject.h"

using std::cout;
using std::endl;

//Constructor(s) & Destructor

/*******************************************************************************/
/*!
\brief
The constructor of Transform.
*/
/*******************************************************************************/
Transform::Transform(GameObject& gameObject) : Component("Transform", gameObject) {
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);
}

/*******************************************************************************/
/*!
\brief
An empty destructor.
*/
/*******************************************************************************/
Transform::~Transform() {
}

//Getter(s)

/*******************************************************************************/
/*!
\brief
Gets the global position.

\return
The position of the GameObject.
*/
/*******************************************************************************/
const Vector3& Transform::GetPosition() const {
	return this->position;
}

/*******************************************************************************/
/*!
\brief
Gets the global rotation.

\return
The rotation of the GameObject.
*/
/*******************************************************************************/
const Vector3& Transform::GetRotation() const {
	return this->rotation;
}

/*******************************************************************************/
/*!
\brief
Gets the global scale.

\return
The scale of the GameObject.
*/
/*******************************************************************************/
const Vector3& Transform::GetScale() const {
	return this->scale;
}

/*******************************************************************************/
/*!
\brief
Gets the forward vector of the GameObject.

\return
The forward vector of the GameObject.
*/
/*******************************************************************************/
Vector3 Transform::GetForward() const {
	Mtx44 transformationMatrix;
	transformationMatrix.SetToTranslation(0, 0, 1);
	transformationMatrix = GetRotationMatrix() * transformationMatrix;	
	return Vector3(transformationMatrix.a[12], transformationMatrix.a[13], transformationMatrix.a[14]);
}

/*******************************************************************************/
/*!
\brief
Gets the up vector of the GameObject.

\return
The up vector of the GameObject.
*/
/*******************************************************************************/
Vector3 Transform::GetUp() const {
	Mtx44 transformationMatrix;
	transformationMatrix.SetToTranslation(0, 1, 0);
	transformationMatrix = GetRotationMatrix() * transformationMatrix;
	return Vector3(transformationMatrix.a[12], transformationMatrix.a[13], transformationMatrix.a[14]);
}

/*******************************************************************************/
/*!
\brief
Gets the left vector of the GameObject.

\return
The left vector of the GameObject.
*/
/*******************************************************************************/
Vector3 Transform::GetLeft() const {
	Mtx44 transformationMatrix;
	transformationMatrix.SetToTranslation(1, 0, 0);
	transformationMatrix = GetRotationMatrix() * transformationMatrix;
	return Vector3(transformationMatrix.a[12], transformationMatrix.a[13], transformationMatrix.a[14]);
}

/*******************************************************************************/
/*!
\brief
Gets the rotation of the GameObject in the form of a Mtx44.

\return
The rotationMatrix of the GameObject.
*/
/*******************************************************************************/
Mtx44 Transform::GetRotationMatrix() const {
	//Mtx44 transformationMatrix[3];
	Mtx44 rotationMatrix[3];
	rotationMatrix[0].SetToRotation(this->rotation.x, 1, 0, 0);
	rotationMatrix[1].SetToRotation(this->rotation.y, 0, 1, 0);
	rotationMatrix[2].SetToRotation(this->rotation.z, 0, 0, 1);
	return rotationMatrix[1] * rotationMatrix[0] * rotationMatrix[2];
}

//Setter(s)

/*******************************************************************************/
/*!
\brief
Sets the position of the GameObject.

\param position
The position of the GameObject.
*/
/*******************************************************************************/
void Transform::SetPosition(const Vector3& position) {
	this->position = position;
}

/*******************************************************************************/
/*!
\brief
Sets the position of the GameObject.

\param x
The position of the GameObject on the x-axis.
\param y
The position of the GameObject on the y-axis.
\param z
The position of the GameObject on the z-axis.
*/
/*******************************************************************************/
void Transform::SetPosition(float x, float y, float z) {
	SetPosition(Vector3(x, y, z));
}

void Transform::SetPositionX(const float x) {
	this->position.x = x;
}

void Transform::SetPositionY(const float y) {
	this->position.y = y;
}

void Transform::SetPositionZ(const float z) {
	this->position.z = z;
}

/*******************************************************************************/
/*!
\brief
Sets the rotation of the GameObject.

\param rotation
The rotation of the GameObject.
*/
/*******************************************************************************/
void Transform::SetRotation(const Vector3& rotation) {
	this->rotation = rotation;
}

/*******************************************************************************/
/*!
\brief
Sets the rotation of the GameObject.

\param x
The rotation of the GameObject on the x-axis.
\param y
The rotation of the GameObject on the y-axis.
\param z
The rotation of the GameObject on the z-axis.
*/
/*******************************************************************************/
void Transform::SetRotation(float x, float y, float z) {
	SetRotation(Vector3(x, y, z));
}

/*******************************************************************************/
/*!
\brief
Sets the scale of the GameObject.

\param scale
The scale of the GameObject.
*/
/*******************************************************************************/
void Transform::SetScale(const Vector3& scale) {
	this->scale = scale;
}

/*******************************************************************************/
/*!
\brief
Sets the scale of the GameObject.

\param x
The scale of the GameObject on the x-axis.
\param y
The scale of the GameObject on the y-axis.
\param z
The scale of the GameObject on the z-axis.
*/
/*******************************************************************************/
void Transform::SetScale(float x, float y, float z) {
	SetScale(Vector3(x, y, z));
}

//Function(s)

/*******************************************************************************/
/*!
\brief
Translate the GameObject.

\param translation
The amount to translate the GameObject by.
*/
/*******************************************************************************/
void Transform::Translate(const Vector3& translation) {
	this->position += translation;
}

/*******************************************************************************/
/*!
\brief
Translate the GameObject.

\param x
The amount to translate the GameObject by on the x-axis.
\param y
The amount to translate the GameObject by on the y-axis.
\param z
The amount to translate the GameObject by on the z-axis.
*/
/*******************************************************************************/
void Transform::Translate(float x, float y, float z) {
	Translate(Vector3(x, y, z));
}

/*******************************************************************************/
/*!
\brief
Rotate the GameObject.

\param rotation
The amount to rotate the GameObject by.
*/
/*******************************************************************************/
void Transform::Rotate(const Vector3& rotation) {
	this->rotation += rotation;
}

/*******************************************************************************/
/*!
\brief
Rotate the GameObject.

\param x
The amount to rotate the GameObject by on the x-axis.
\param y
The amount to rotate the GameObject by on the y-axis.
\param z
The amount to rotate the GameObject by on the z-axis.
*/
/*******************************************************************************/
void Transform::Rotate(float x, float y, float z) {
	Rotate(Vector3(x, y, z));
}

/*******************************************************************************/
/*!
\brief
Scale the GameObject on its 3 axis.

\param scale
The amount to scale the GameObject by.
*/
/*******************************************************************************/
void Transform::Scale(const Vector3& scale) {
	this->scale.x *= scale.x;
	this->scale.y *= scale.y;
	this->scale.z *= scale.z;
}

/*******************************************************************************/
/*!
\brief
Scale the GameObject on its 3 axis.

\param x
The amount to scale the GameObject by on the x-axis.
\param y
The amount to scale the GameObject by on the y-axis.
\param z
The amount to scale the GameObject by on the z-axis.
*/
/*******************************************************************************/
void Transform::Scale(float x, float y, float z) {
	Scale(Vector3(x, y, z));
}

/*******************************************************************************/
/*!
\brief
Scale the GameObject uniformly.

\param scale
The amount to scale the GameObject by.
*/
/*******************************************************************************/
void Transform::Scale(float scale) {
	this->scale *= scale;
}

/*******************************************************************************/
/*!
\brief
Rotates the transform so the forward vector points at target's current position.
Then it rotates the transform to point its up direction vector in the direction hinted at by the worldUp vector.
If you leave out the up parameter, the function will use the world y axis.
up is only a hint vector. The up vector of the rotation will only match the up vector if the forward direction is perpendicular to up.

\param target
The target' position.
\param up
The fucking up vector the fucking shit is supposed to orient to.
*/
/*******************************************************************************/
void Transform::LookAt(Vector3 target, Vector3 up) {
	Vector3 view = target - position;
	Vector3 left = up.Cross(view);
	up = view.Cross(left);
	try {
		view.Normalize();
		left.Normalize();
		up.Normalize();
	} catch (DivideByZero) {
		cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		return;
	}	
	if (view.IsZero() || left.IsZero() || up.IsZero()) {
		cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		return;
	}

	//First we need to rotate on the Y-Axis until our View aligns with the YZ-Plane.
	//To do that we need our projection on the XZ-Plane, which is the View vector without it's Y-Value.
	float y = Math::RadianToDegree(atan2(view.x, view.z));
	//Then, we need to find our how much we rotated on the X-Axis.
	float x = -Math::RadianToDegree(asin(view.y));
	//To find our Z-Rotation, we first need to know what our left vector would be if we had a Z-Rotation of 0,
	//The expected left vector if Z-Rotation is 0:
	Vector3 expectedLeft(view.z, 0, -view.x);
	//The expected up vector id Z-Rotation is 0:
	Vector3 expectedUp = view.Cross(expectedLeft);
	float z = 0.0f;
	try {
		z = Math::RadianToDegree(atan2(left.Dot(expectedUp.Normalize()), left.Dot(expectedLeft.Normalize())));
	} catch (DivideByZero) {
		cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		return;
	}

	rotation.Set(x, y, z);
}