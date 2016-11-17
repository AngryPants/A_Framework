#ifndef UTILITY_H
#define UTILITY_H

#include "Mtx44.h"
#include "../Mesh/Vertex.h"

Position operator*(const Mtx44& lhs, const Position& rhs);
Position ToPosition(const Vector3& vector);

#endif