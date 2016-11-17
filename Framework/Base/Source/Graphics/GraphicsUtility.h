#ifndef GRAPHICS_UTILITY_H
#define GRAPHICS_UTILITY_H

#include "ShaderProgram.h"
#include "../Mesh/Material.h"
#include "../Mesh/Vertex.h"
#include "Mtx44.h"

ShaderProgram* GetShader();

ShaderProgram::FloatMat4x4 ToOpenglStruct(const Mtx44& matrix);

ShaderProgram::FloatVec3 ToOpenglStruct(const Color& color);

ShaderProgram::FloatVec3 ToOpenglStruct(const MaterialComponent& component);

ShaderProgram::FloatVec3 ToOpenglStruct(const Vector3& vector);

ShaderProgram::FloatVec3 ToOpenglStruct(const Position& position);

#endif