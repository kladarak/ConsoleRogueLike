#include "CollisionMesh.h"

CollisionMesh::CollisionMesh(const bool* inCollidableArray, size_t inCols, size_t inRows, const IVec2& inCentreOffset)
	: Dynamic2DVectorWithOffset<bool>()
{
	for (size_t row = 0; row < inRows; ++row)
	{
		for (size_t col = 0; col < inCols; ++col)
		{
			Set(col, row, inCollidableArray[(inCols*row) + col]);
		}
	}

	SetCentreOffset(inCentreOffset);
}
