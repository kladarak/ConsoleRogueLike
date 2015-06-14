#include "AsciiMesh.h"


AsciiMesh::AsciiMesh(const char* inChars, size_t inCols, size_t inRows, const IVec2& inCentreOffset)
	: mCentreOffset(inCentreOffset)
{
	for (size_t row = 0; row < inRows; ++row)
	{
		for (size_t col = 0; col < inCols; ++col)
		{
			mFragments.Set(col, row, inChars[(inCols*row) + col]);
		}
	}
}

AsciiMesh::AsciiMesh(char inChar)
	: mCentreOffset(0, 0)
{
	mFragments.Set(0, 0, inChar);
}