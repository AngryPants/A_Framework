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
	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;
	
public:
	//Constructor(s) & Destructor
	Transform(GameObject& gameObject);
	virtual ~Transform();

	//Getter(s)
	//Get the position, rotation and scale of the transform relative to the parent.
	const Vector3& GetLocalPosition() const;
	const Vector3& GetLocalRotation() const;
	const Vector3& GetLocalScale() const;

	Vector3 GetPosition() const;

	//Get the Forward, Up and Left Vectors of the transform relative to the world.
	Vector3 GetForward() const;
	Vector3 GetUp() const;
	Vector3 GetLeft() const;

	//Get the Forward, Up and Left Vectors of the transform relative to the parent.
	Vector3 GetLocalForward() const;
	Vector3 GetLocalUp() const;
	Vector3 GetLocalLeft() const;

	//Get the Rotation, Translation, Scale and Transformation matrix relative to the parent.
	Mtx44 GetRotationMatrix() const;
	Mtx44 GetTranslationMatrix() const;
	Mtx44 GetScaleMatrix() const;
	Mtx44 GetTransformationMatrix() const;

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
	void Translate(const Vector3& translation);
	void Translate(float x, float y, float z);
	void Rotate(const Vector3& rotation);
	void Rotate(float x, float y, float z);
	void Scale(const Vector3& scale);
	void Scale(float x, float y, float z);
	//void LookAt(Vector3 target, Vector3 up = Vector3(0, 1, 0));

};

#endif