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

AsciiMesh::AsciiMesh(const Fragment* inFrags, size_t inCols, size_t inRows, const IVec2& inCentreOffset)
	: Base()
{
	for (size_t row = 0; row < inRows; ++row)
	{
		for (size_t col = 0; col < inCols; ++col)
		{
			Set(col, row, inFrags[(inCols*row) + col]);
		}
	}

	SetCentreOffset(inCentreOffset);
}

AsciiMesh gReColourAsciiMesh(const AsciiMesh& inMesh, EColour inColour)
{
	AsciiMesh newMesh;
	newMesh.SetCentreOffset( inMesh.GetCentreOffset() );

	inMesh.ForEachFrag( [&] (size_t inCol, size_t inRow, const Fragment& inFrag)
	{
		Fragment newFrag = inFrag;
		newFrag.mColour = inColour;
		newMesh.Set(inCol, inRow, newFrag);
	} );

	return newMesh;
}
