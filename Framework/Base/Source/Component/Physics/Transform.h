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
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

public:
	//Constructor(s)
	Transform(GameObject& gameObject);

	//Destructor
	virtual ~Transform();

	//Getter(s)
	const Vector3& GetPosition() const;
	const Vector3& GetRotation() const;
	const Vector3& GetScale() const;

	Vector3 GetForward() const;
	Vector3 GetUp() const;
	Vector3 GetLeft() const;
	
	Mtx44 GetRotationMatrix() const;

	//Setters(s)
	void SetPosition(const Vector3& position);
	void SetPosition(float x, float y, float z);
	void SetPositionX(const float x);
	void SetPositionY(const float y);
	void SetPositionZ(const float z);
	void SetRotation(const Vector3& rotation);
	void SetRotation(float x, float y, float z);
	void SetScale(const Vector3& scale);
	void SetScale(float x, float y, float z);
	
	//Function(s)
	void Translate(const Vector3& translation);
	void Translate(float x, float y, float z);
	void Rotate(const Vector3& rotation);
	void Rotate(float x, float y, float z);
	void Scale(const Vector3& scale);
	void Scale(float x, float y, float z);
	void Scale(float scale);
	void LookAt(Vector3 target, Vector3 up = Vector3(0, 1, 0));

};

#endif