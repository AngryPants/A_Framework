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
#include "../ComponentManager.h"

using std::cout;
using std::endl;

//Constructor(s) & Destructor

/*******************************************************************************/
/*!
\brief
The constructor of Transform.
*/
/*******************************************************************************/
Transform::Transform(GameObject& _gameObject, PassKey<ComponentManager> _key) : Component("Transform", _gameObject, _key) {
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

void Transform::SetDirty() {
	if (isDirty == false) {
		isDirty = true;
		vector<GameObject*> childrenList;
		GetGameObject().GetChildren(childrenList);
		for (vector<GameObject*>::iterator vecIter = childrenList.begin(); vecIter != childrenList.end(); ++vecIter) {
			GameObject* goPtr = *vecIter;
			goPtr->GetComponent<Transform>().SetDirty();
		}
	}	
}

bool Transform::IsDirty() {
	GameObject* parent = GetGameObject().GetParent();
	if (parent != nullptr) {
		isDirty |= parent->GetComponent<Transform>().IsDirty();
	}

	return isDirty;
}

//Calculations
void Transform::Calculate() {
	//These functions must be called in this specific order due to certain matrices/vectors depending on others.

	//Local
	//Matrices
	CalculateLocalMatrices();
	
	//Direction Vectors
	CalculateLocalVectors();	
	
	//Global
	//Matrices
	CalculateTransformationMatrix();
	CalculateRotationMatrix();

	//Direction Vectors
	CalculateGlobalVectors();

	//Transformation
	CalculatePosition();
	CalculateRotation();

	isDirty = false;
}

//Local (Relative To Parent)
//Matrices
void Transform::CalculateLocalMatrices() {
	//Translation
	localTranslationMatrix.SetToTranslation(this->localPosition.x, this->localPosition.y, this->localPosition.z);

	//Rotation
	Mtx44 mat[3];
	mat[0].SetToRotation(this->localRotation.x, 1, 0, 0);
	mat[1].SetToRotation(this->localRotation.y, 0, 1, 0);
	mat[2].SetToRotation(this->localRotation.z, 0, 0, 1);
	localRotationMatrix = mat[1] * mat[0] * mat[2]; //Order of Rotation - Z, X, Y

	//Scale
	localScaleMatrix.SetToScale(localScale.x, localScale.y, localScale.z);

	//All 3 Combined
	localTransformationMatrix = localTranslationMatrix * localRotationMatrix * localScaleMatrix;
}

//Direction Vectors
void Transform::CalculateLocalVectors() {
	Vector3 direction;

	//Forward
	direction.Set(0, 0, 1);
	localForward = localRotationMatrix * direction;

	//Up
	direction.Set(0, 1, 0);
	localUp = localRotationMatrix * direction;

	//Left
	direction.Set(1, 0, 0);
	localLeft = localRotationMatrix * direction;
}

//Global
//Matrices
void Transform::CalculateTransformationMatrix() {
	GameObject* parent = GetGameObject().GetParent();
	if (parent == nullptr) {
		transformationMatrix = localTransformationMatrix;
	} else {
		transformationMatrix = parent->GetComponent<Transform>().GetTransformationMatrix() * localTransformationMatrix;
	}
}

void Transform::CalculateRotationMatrix() {
	GameObject* parent = GetGameObject().GetParent();
	if (parent == nullptr) {
		rotationMatrix = localRotationMatrix;
	} else {
		rotationMatrix = parent->GetComponent<Transform>().GetRotationMatrix() * localRotationMatrix;
	}
}

//Transformation
void Transform::CalculatePosition() {
	//Make sure that this is called after CalculateTransformationMatrix.
	GameObject* parent = GetGameObject().GetParent();
	if (parent == nullptr) {
		position = localPosition;
	} else {
		Mtx44 mat = parent->GetComponent<Transform>().GetTransformationMatrix() * localTranslationMatrix;
		position.Set(mat.a[12], mat.a[13], mat.a[14]);
	}
}

/*void Transform::CalculateRotation() {
	//Make sure that this is called after Calculating global direction vectors.
	GameObject* parent = GetGameObject().GetParent();
	if (parent == nullptr) {
		rotation = localRotation;
	} else {
		//Imagine we start off with rotation of 0, 0, 0
		//First we need to rotate on the Y-Axis until our forward aligns with the YZ-Plane.
		//To do that we need our projection on the XZ-Plane, which is the View vector without it's Y-Value.
		float y = Math::RadianToDegree(atan2(forward.x, forward.z));
		//Then, we need to find our how much we rotated on the X-Axis.
		float x = Math::RadianToDegree(-asin(forward.y));
		//To find our Z-Rotation, we first need to know what our left vector would be if we had a Z-Rotation of 0,
		//The expected left vector if Z-Rotation is 0:
		Vector3 expectedLeft(forward.z, 0, -forward.x);
		//The expected up vector id Z-Rotation is 0:
		Vector3 expectedUp = forward.Cross(expectedLeft);
		float z = 0.0f;
		try {
			z = Math::RadianToDegree(atan2(left.Dot(expectedUp.Normalize()), left.Dot(expectedLeft.Normalize())));
		} catch (DivideByZero) {
			//Do Nothing
		}

		rotation.Set(x, y, z);
	}
}*/

void Transform::CalculateRotation() {
}

//Direction Vectors
void Transform::CalculateGlobalVectors() {
	GameObject* parent = GetGameObject().GetParent();
	if (parent == nullptr) {
		forward = localForward;
		up = localUp;
		left = localLeft;
	} else {
		//Mtx44 mat = parent->GetComponent<Transform>().GetRotationMatrix() * localRotationMatrix;
		forward = rotationMatrix * Vector3(0, 0, 1);
		up = rotationMatrix * Vector3(0, 1, 0);
		left = rotationMatrix * Vector3(1, 0, 0);
	}
}

//Local
const Vector3& Transform::GetLocalPosition() const {
	return this->localPosition;
}

const Vector3& Transform::GetLocalRotation() const {
	return this->localRotation;
}

const Vector3& Transform::GetLocalScale() const {
	return this->localScale;
}

//Matrices
Mtx44 Transform::GetLocalRotationMatrix() {
	if (IsDirty()) {
		Calculate();
	}

	return localRotationMatrix;
}

Mtx44 Transform::GetLocalTranslationMatrix() {
	if (IsDirty()) {
		Calculate();
	}

	return localTransformationMatrix;
}

Mtx44 Transform::GetLocalScaleMatrix() {
	if (IsDirty()) {
		Calculate();
	}

	return localScaleMatrix;
}

Mtx44 Transform::GetLocalTransformationMatrix() {
	if (IsDirty()) {
		Calculate();
	}

	return localTransformationMatrix;
}

//Direction Vectors
/*******************************************************************************/
/*!
\brief
Gets the forward vector of the GameObject relative to the parent.

\return
The forward vector of the GameObject relative to the parent.
*/
/*******************************************************************************/
Vector3 Transform::GetLocalForward() {
	if (IsDirty()) {
		Calculate();
	}

	return localForward;
}

/*******************************************************************************/
/*!
\brief
Gets the up vector of the GameObject relative to the parent.

\return
The up vector of the GameObject relative to the parent.
*/
/*******************************************************************************/
Vector3 Transform::GetLocalUp() {
	if (IsDirty()) {
		Calculate();
	}

	return localUp;
}

/*******************************************************************************/
/*!
\brief
Gets the left vector of the GameObject relative to the parent.

\return
The left vector of the GameObject relative to the parent.
*/
/*******************************************************************************/
Vector3 Transform::GetLocalLeft() {
	if (IsDirty()) {
		Calculate();
	}

	return localLeft;
}

//Global
Vector3 Transform::GetPosition() {
	if (IsDirty()) {
		Calculate();
	}

	return position;
}

/*Vector3 Transform::GetRotation() {
	if (IsDirty()) {
		Calculate();
	}

	return rotation;
}*/

Vector3 Transform::GetForward() {
	if (IsDirty()) {
		Calculate();
	}

	return forward;
}

Vector3 Transform::GetUp() {
	if (IsDirty()) {
		Calculate();
	}

	return up;
}

Vector3 Transform::GetLeft() {
	if (IsDirty()) {
		Calculate();
	}

	return left;
}

//Global
Mtx44 Transform::GetTransformationMatrix() {
	if (IsDirty()) {
		Calculate();
	}

	return transformationMatrix;
}

Mtx44 Transform::GetRotationMatrix() {
	if (IsDirty()) {
		Calculate();
	}

	return rotationMatrix;
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
	SetDirty();
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
	SetDirty();
}

void Transform::SetLocalPositionY(const float y) {
	this->localPosition.y = y;
	SetDirty();
}

void Transform::SetLocalPositionZ(const float z) {
	this->localPosition.z = z;
	SetDirty();
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
	SetDirty();
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
	SetDirty();
}

void Transform::SetLocalRotationX(float x) {
	this->localRotation.x = x;
	SetDirty();
}

void Transform::SetLocalRotationY(float y) {
	this->localRotation.y = y;
	SetDirty();
}

void Transform::SetLocalRotationZ(float z) {
	this->localRotation.z = z;
	SetDirty();
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
	SetDirty();
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
	SetDirty();
}

void Transform::SetLocalScaleY(float y) {
	this->localScale.y = y;
	SetDirty();
}

void Transform::SetLocalScaleZ(float z) {
	this->localScale.z = z;
	SetDirty();
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
	SetDirty();
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
	SetDirty();
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
	SetDirty();
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
The up vector the transform is supposed to orient to.
*/
/*******************************************************************************/
/*void Transform::LookAt(Vector3 _target, Vector3 _up) {
	//Ensure that the parameters are valid.
	Vector3 desiredView = _target - position;
	Vector3 desiredLeft = _up.Cross(desiredView);
	_up = desiredLeft.Cross(desiredLeft);
	try {
		desiredView.Normalize();
		desiredLeft.Normalize();
		_up.Normalize();
	} catch (DivideByZero) {
		cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		return;
	}	
	if (desiredView.IsZero() || desiredLeft.IsZero() || _up.IsZero()) {
		cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		return;
	}

	//Let's move on to calculations. We want to find our what our global rotation would be if we were looking at the target.
	//First we need to rotate on the Y-Axis until our View aligns with the YZ-Plane.
	//To do that we need our projection on the XZ-Plane, which is the View vector without it's Y-Value.
	float y = Math::RadianToDegree(atan2(desiredView.x, desiredView.z));
	//Then, we need to find our how much we rotated on the X-Axis.
	float x = Math::RadianToDegree(-asin(desiredView.y));
	//To find our Z-Rotation, we first need to know what our left vector would be if we had a Z-Rotation of 0,
	//The expected left vector if Z-Rotation is 0:
	Vector3 expectedLeft(desiredView.z, 0, -desiredView.x);
	//The expected up vector id Z-Rotation is 0:
	Vector3 expectedUp = desiredView.Cross(expectedLeft);
	float z = 0.0f;
	try {
		z = Math::RadianToDegree(atan2(desiredView.Dot(expectedUp.Normalize()), desiredView.Dot(expectedLeft.Normalize())));
	} catch (DivideByZero) {
		cout << "[Transform::LookAt(Vector3 target, Vector3 up) Error]" << endl;
		return;
	}

	//Now we know that we would need to be looking at Vector3(x, y, z) for our global rotation.
	//However, since the whole transform relies on our localRotation, we need to convert it.

}*/

void Transform::SetInactive() {
	this->isActive = true;
}

void Transform::SetActive() {
	this->isActive = true;
}