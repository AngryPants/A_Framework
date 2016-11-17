#include "GraphicsUtility.h"
#include "GraphicsManager.h"

ShaderProgram* GetShader() {
	ShaderProgram* shaderProgram = GraphicsManager::GetInstance().GetActiveShader();
	if (shaderProgram == nullptr) {
		throw std::exception("Unable to render as no active shader has been set.");
	}
	return shaderProgram;	
}

ShaderProgram::FloatMat4x4 ToOpenglStruct(const Mtx44& matrix) {
	ShaderProgram::FloatMat4x4 mat;
	for (unsigned int i = 0; i < 16; ++i) {
		mat.mat[i] = matrix.a[i];
	}

	return mat;
}

ShaderProgram::FloatVec3 ToOpenglStruct(const Color& color) {
	ShaderProgram::FloatVec3 col;
	col.vec[0] = color.r;
	col.vec[1] = color.g;
	col.vec[2] = color.b;

	return col;
}

ShaderProgram::FloatVec3 ToOpenglStruct(const MaterialComponent& component) {
	ShaderProgram::FloatVec3 comp;
	comp.vec[0] = component.r;
	comp.vec[1] = component.g;
	comp.vec[2] = component.b;

	return comp;
}

ShaderProgram::FloatVec3 ToOpenglStruct(const Vector3& vector) {
	ShaderProgram::FloatVec3 vect;
	vect.vec[0] = vector.x;
	vect.vec[1] = vector.y;
	vect.vec[2] = vector.z;

	return vect;
}

ShaderProgram::FloatVec3 ToOpenglStruct(const Position& position) {
	ShaderProgram::FloatVec3 pos;
	pos.vec[0] = position.x;
	pos.vec[1] = position.y;
	pos.vec[2] = position.z;

	return pos;
}