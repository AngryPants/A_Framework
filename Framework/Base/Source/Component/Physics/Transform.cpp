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
	SetLocalPosition(0, 0, 0);
	SetLocalRotation(0, 0, 0);
	SetLocalScale(1, 1, 1);
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
const Vector3& Transform::GetLocalPosition() const {
	return this->localPosition;
}

/*******************************************************************************/
/*!
\brief
Gets the global rotation.

\return
The rotation of the GameObject.
*/
/*******************************************************************************/
const Vector3& Transform::GetLocalRotation() const {
	return this->localRotation;
}

/*******************************************************************************/
/*!
\brief
Gets the global scale.

\return
The scale of the GameObject.
*/
/*******************************************************************************/
const Vector3& Transform::GetLocalScale() const {
	return this->localScale;
}

Vector3 Transform::GetPosition() const {
	GameObject* parentPtr = GetGameObject().GetParent();
	if (parentPtr == nullptr) {
		return localPosition;
	}

	Mtx44 transformationMatrix = GetTranslationMatrix();
	while (parentPtr != nullptr) {
		//transformationMatrix = transformationMatrix * parentPtr->GetComponent<Transform>().GetTransformationMatrix();
		transformationMatrix = parentPtr->GetComponent<Transform>().GetTransformationMatrix() * transformationMatrix;
		parentPtr = parentPtr->GetParent();		
	}
	return Vector3(transformationMatrix.a[12], transformationMatrix.a[13], transformationMatrix.a[14]);
}

Vector3 Transform::GetForward() const {
	Mtx44 rotationMatrix = GetRotationMatrix();
	GameObject* parentPtr = GetGameObject().GetParent();
	while (parentPtr != nullptr) {
		//rotationMatrix = rotationMatrix * parentPtr->GetComponent<Transform>().GetRotationMatrix();
		rotationMatrix = parentPtr->GetComponent<Transform>().GetRotationMatrix() * rotationMatrix; //WHY DOES  THIS WORK?!
		parentPtr = parentPtr->GetParent();
	}
	return rotationMatrix * Vector3(0, 0, 1);
}

Vector3 Transform::GetUp() const {
	Mtx44 rotationMatrix = GetRotationMatrix();
	GameObject* parentPtr = GetGameObject().GetParent();
	while(parentPtr != nullptr) {
		//rotationMatrix = rotationMatrix * parentPtr->GetComponent<Transform>().GetRotationMatrix();
		rotationMatrix = parentPtr->GetComponent<Transform>().GetRotationMatrix() * rotationMatrix; //WHY DOES  THIS WORK?!
		parentPtr = parentPtr->GetParent();
	}
	return rotationMatrix * Vector3(0, 1, 0);
}

Vector3 Transform::GetLeft() const {
	Mtx44 rotationMatrix = GetRotationMatrix();
	GameObject* parentPtr = GetGameObject().GetParent();
	while(parentPtr != nullptr) {
		//rotationMatrix = rotationMatrix * parentPtr->GetComponent<Transform>().GetRotationMatrix();
		rotationMatrix = parentPtr->GetComponent<Transform>().GetRotationMatrix() * rotationMatrix; //WHY DOES  THIS WORK?!
		parentPtr = parentPtr->GetParent();
	}
	return rotationMatrix * Vector3(1, 0, 0);
}

/*******************************************************************************/
/*!
\brief
Gets the forward vector of the GameObject relative to the parent.

\return
The forward vector of the GameObject relative to the parent.
*/
/*******************************************************************************/
Vector3 Transform::GetLocalForward() const {
	Mtx44 transformationMatrix;
	transformationMatrix.SetToTranslation(0, 0, 1);
	transformationMatrix = GetRotationMatrix() * transformationMatrix;
	return Vector3(transformationMatrix.a[12], transformationMatrix.a[13], transformationMatrix.a[14]);
}

/*******************************************************************************/
/*!
\brief
Gets the up vector of the GameObject relative to the parent.

\return
The up vector of the GameObject relative to the parent.
*/
/*******************************************************************************/
Vector3 Transform::GetLocalUp() const {
	Mtx44 transformationMatrix;
	transformationMatrix.SetToTranslation(0, 1, 0);
	transformationMatrix = GetRotationMatrix() * transformationMatrix;
	return Vector3(transformationMatrix.a[12], transformationMatrix.a[13], transformationMatrix.a[14]);
}

/*******************************************************************************/
/*!
\brief
Gets the left vector of the GameObject relative to the parent.

\return
The left vector of the GameObject relative to the parent.
*/
/*******************************************************************************/
Vector3 Transform::GetLocalLeft() const {
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
	Mtx44 rotationMatrix[3];
	rotationMatrix[0].SetToRotation(this->localRotation.x, 1, 0, 0);
	rotationMatrix[1].SetToRotation(this->localRotation.y, 0, 1, 0);
	rotationMatrix[2].SetToRotation(this->localRotation.z, 0, 0, 1);
	return rotationMatrix[1] * rotationMatrix[0] * rotationMatrix[2]; //Order of Rotation - Z, X, Y
}

Mtx44 Transform::GetTranslationMatrix() const {
	Mtx44 translationMatrix;
	translationMatrix.SetToTranslation(this->localPosition.x, this->localPosition.y, this->localPosition.z);	
	return translationMatrix;
}

Mtx44 Transform::GetScaleMatrix() const {
	Mtx44 scaleMatrix;
	scaleMatrix.SetToScale(localScale.x, localScale.y, localScale.z);
	return scaleMatrix;
}

Mtx44 Transform::GetTransformationMatrix() const {
	return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
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
void Transform::SetLocalPosition(const Vector3& position) {
	this->localPosition = position;
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
void Transform::SetLocalPosition(float x, float y, float z) {
	SetLocalPosition(Vector3(x, y, z));
}

void Transform::SetLocalPositionX(const float x) {
	this->localPosition.x = x;
}

void Transform::SetLocalPositionY(const float y) {
	this->localPosition.y = y;
}

void Transform::SetLocalPositionZ(const float z) {
	this->localPosition.z = z;
}

/*******************************************************************************/
/*!
\brief
Sets the rotation of the GameObject.

\param rotation
The rotation of the GameObject.
*/
/*******************************************************************************/
void Transform::SetLocalRotation(const Vector3& rotation) {
	this->localRotation = rotation;
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
void Transform::SetLocalRotation(float x, float y, float z) {
	SetLocalRotation(Vector3(x, y, z));
}

void Transform::SetLocalRotationX(float x) {
	this->localRotation.x = x;
}

void Transform::SetLocalRotationY(float y) {
	this->localRotation.y = y;
}

void Transform::SetLocalRotationZ(float z) {
	this->localRotation.z = z;
}

/*******************************************************************************/
/*!
\brief
Sets the scale of the GameObject.

\param scale
The scale of the GameObject.
*/
/*******************************************************************************/
void Transform::SetLocalScale(const Vector3& scale) {
	this->localScale = scale;

	if(Vector::IsEqual(this->localScale.x,0.0f)) {
		if(this->localScale.x >= 0.0f) {
			this->localScale.x = Math::EPSILON;
		} else {
			this->localScale.x = -Math::EPSILON;
		}
	}
	if(Vector::IsEqual(this->localScale.y,0.0f)) {
		if(this->localScale.y >= 0.0f) {
			this->localScale.y = Math::EPSILON;
		} else {
			this->localScale.y = -Math::EPSILON;
		}
	}
	if(Vector::IsEqual(this->localScale.z,0.0f)) {
		if(this->localScale.z >= 0.0f) {
			this->localScale.z = Math::EPSILON;
		} else {
			this->localScale.z = -Math::EPSILON;
		}
	}
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
void Transform::SetLocalScale(float x, float y, float z) {
	SetLocalScale(Vector3(x, y, z));
}

void Transform::SetLocalScaleX(float x) {
	this->localScale.x = x;
	if (Vector::IsEqual(localScale.x, 0.0f)) {
		if (localScale.x >= 0.0f) {
			localScale.x = Math::EPSILON;
		} else {
			localScale.x = -Math::EPSILON;
		}
	}
}

void Transform::SetLocalScaleY(float y) {
	this->localScale.y = y;
	if(Vector::IsEqual(localScale.y,0.0f)) {
		if(localScale.y >= 0.0f) {
			localScale.y = Math::EPSILON;
		} else {
			localScale.y = -Math::EPSILON;
		}
	}
}

void Transform::SetLocalScaleZ(float z) {
	this->localScale.z = z;
	if(Vector::IsEqual(localScale.z,0.0f)) {
		if(localScale.z >= 0.0f) {
			localScale.z = Math::EPSILON;
		} else {
			localScale.z = -Math::EPSILON;
		}
	}
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
	this->localPosition += translation;
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
	this->localRotation += rotation;
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
	this->localScale.x *= scale.x;
	this->localScale.y *= scale.y;
	this->localScale.z *= scale.z;

	if(Vector::IsEqual(this->localScale.x,0.0f)) {
		if(this->localScale.x >= 0.0f) {
			this->localScale.x = Math::EPSILON;
		} else {
			this->localScale.x = -Math::EPSILON;
		}
	}
	if(Vector::IsEqual(this->localScale.y,0.0f)) {
		if(this->localScale.y >= 0.0f) {
			this->localScale.y = Math::EPSILON;
		} else {
			this->localScale.y = -Math::EPSILON;
		}
	}
	if(Vector::IsEqual(this->localScale.z,0.0f)) {
		if(this->localScale.z >= 0.0f) {
			this->localScale.z = Math::EPSILON;
		} else {
			this->localScale.z = -Math::EPSILON;
		}
	}
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
/*void Transform::LookAt(Vector3 target, Vector3 up) {
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
}*/