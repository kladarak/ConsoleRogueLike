#pragma once

#include <vector>

#define gElemCount( arr ) (sizeof(arr)/sizeof(arr[0]))

template<typename T>
inline std::vector<T> gCArrayToVector(const T* inArray, size_t inElemCount)
{
	std::vector<T> out;
	out.reserve(inElemCount);

	for (size_t i = 0; i < inElemCount; ++i)
	{
		out.push_back( inArray[i] );
	}

	return out;
}