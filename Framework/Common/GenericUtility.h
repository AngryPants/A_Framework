#ifndef GENERIC_UTILITY_H
#define GENERIC_UTILITY_H

template <typename Type>
bool IsType(void* _objectPtr) {
	return (dynamic_cast<Type*>(_objectPtr) != nullptr);
}

#endif