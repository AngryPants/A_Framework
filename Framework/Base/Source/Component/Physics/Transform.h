/*******************************************************************************/
/*!
\file Transform.h
\author Lim Ngian Xin Terry
\par email: lnxterry\@gmail.com

\brief
Header file for Transform.
*/
/*******************************************************************************/

#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "../Component.h"
#include "Vector3.h"
#include "MatrixStack.h"
#include <set>

using std::set;

/*******************************************************************************/
/*!
Class Transform
\brief
Defines Transform, a Component which stores the transformation of an object.
*/
/*******************************************************************************/
class Transform : public Component {

private:
	//Variable(s)
	//If true, our information is outdated.
	bool isDirty;

	//If true, we'll always be in the extra grid in spatial partition.
	bool ignoresSpatialPartition;

	///**************************Local (Relative To Parent)**************************///
	//Transformations
	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;
	
	//Matrices.
	Mtx44 localRotationMatrix;
	Mtx44 localTranslationMatrix;
	Mtx44 localScaleMatrix;
	Mtx44 localTransformationMatrix;

	//Direction Vectors
	Vector3 localForward;
	Vector3 localUp;
	Vector3 localLeft;
	///******************************************************************************///

	///**************************Global (Relative To World)**************************///
	//Matrices
	Mtx44 transformationMatrix;
	Mtx44 rotationMatrix;

	//Transformations
	Vector3 position;
	Vector3 rotation;

	//Direction Vectors
	Vector3 forward;
	Vector3 up;
	Vector3 left;
	///******************************************************************************///

	//Calculations
	void Calculate();

	///**************************Local (Relative To Parent)**************************///
	//Direction Vectors
	void CalculateLocalVectors();

	//Matrices
	void CalculateLocalMatrices();
	///******************************************************************************///

	///**************************Global (Relative To World)**************************///
	//Transformation
	void CalculatePosition();
	void CalculateRotation();

	//Direction Vectors
	void CalculateGlobalVectors();

	//Matrices
	void CalculateTransformationMatrix();
	void CalculateRotationMatrix();
	///******************************************************************************///

	//We now have outdated information.
	void SetDirty();

public:
	//Constructor(s) & Destructor
	Transform(GameObject& _gameObject, PassKey<ComponentManager> _key);
	virtual ~Transform();

	//Check if our data is outdated.
	bool IsDirty();

	//Check if we ignore Spatial Partitioning.
	bool IgnoresSpatialPartition();
	void IgnoreSpatialPartition(const bool _ignore);

	//Getter(s)
	//Get the position, rotation and scale of the transform relative to the parent.
	const Vector3& GetLocalPosition() const;
	const Vector3& GetLocalRotation() const;
	const Vector3& GetLocalScale() const;

	//Get the position relative to the world.
	Vector3 GetPosition();
	//Vector3 GetRotation();

	//Get the Forward, Up and Left Vectors of the transform relative to the world.
	Vector3 GetForward();
	Vector3 GetUp();
	Vector3 GetLeft();

	//Get the Forward, Up and Left Vectors of the transform relative to the parent.
	Vector3 GetLocalForward();
	Vector3 GetLocalUp();
	Vector3 GetLocalLeft();

	//Get the Rotation, Translation, Scale and Transformation matrix relative to the parent.
	Mtx44 GetLocalRotationMatrix();
	Mtx44 GetLocalTranslationMatrix();
	Mtx44 GetLocalScaleMatrix();
	Mtx44 GetLocalTransformationMatrix();

	Mtx44 GetTransformationMatrix();
	Mtx44 GetRotationMatrix();

	//Setters(s)
	void SetLocalPosition(const Vector3& position);
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPositionX(const float x);
	void SetLocalPositionY(const float y);
	void SetLocalPositionZ(const float z);
	void SetLocalRotation(const Vector3& rotation);
	void SetLocalRotation(float x, float y, float z);
	void SetLocalRotationX(float x);
	void SetLocalRotationY(float y);
	void SetLocalRotationZ(float z);
	void SetLocalScale(const Vector3& scale);
	void SetLocalScale(float x, float y, float z);
	void SetLocalScaleX(float x);
	void SetLocalScaleY(float y);
	void SetLocalScaleZ(float z);
	
	//Function(s)
	//Translate, Rotate and Scale the transform relative to the parent.
	void Translate(const Vector3& translation);
	void Translate(float x, float y, float z);
	void Rotate(const Vector3& rotation);
	void Rotate(float x, float y, float z);
	void Scale(const Vector3& scale);
	void Scale(float x, float y, float z);
	//void LookAt(Vector3 _target, Vector3 _up = Vector3(0, 1, 0));

	virtual void SetInactive();
	virtual void SetActive();

};

#endif