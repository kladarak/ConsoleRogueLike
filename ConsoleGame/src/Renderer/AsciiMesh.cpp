#include "AsciiMesh.h"

AsciiMesh::AsciiMesh(const char* inChars, size_t inCols, size_t inRows, const IVec2& inCentreOffset)
	: Base()
{
	for (size_t row = 0; row < inRows; ++row)
	{
		for (size_t col = 0; col < inCols; ++col)
		{
			Set(col, row, inChars[(inCols*row) + col]);
		}
	}

	SetCentreOffset(inCentreOffset);
}
