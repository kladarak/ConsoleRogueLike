#include "RenderTarget.h"

#include "AsciiMesh.h"

RenderTarget::RenderTarget(int inColumns, int inRows)
	: mFragments(inColumns, inRows)
{
}

void RenderTarget::Set(int inCol, int inRow, char inChar)
{
	if (inCol >= 0 && inCol < GetColCount() && inRow >= 0 && inRow < GetRowCount())
	{
		mFragments.Set(inCol, inRow, inChar);
	}
}

void RenderTarget::Set(int inCol, int inRow, const Fragment& inFragment)
{
	if (inCol >= 0 && inCol < GetColCount() && inRow >= 0 && inRow < GetRowCount())
	{
		mFragments.Set(inCol, inRow, inFragment);
	}
}

bool RenderTarget::operator==(const RenderTarget& inRHS)
{
	if (GetColCount() != inRHS.GetColCount()) return false;
	if (GetRowCount() != inRHS.GetRowCount()) return false;

	bool isEqual = true;

	mFragments.ForEach( [&] (size_t inCol, size_t inRow, const Fragment& inFragment)
	{
		isEqual &= inRHS.mFragments.Get(inCol, inRow) == inFragment;
	} );

	return true;
}

//----------------------------------------------------------------------------------------

void RenderTarget::Write(char inChar, int inX, int inY)
{
	Write(Fragment(inChar), inX, inY);
}

void RenderTarget::Write(const Fragment& inFragment, int inX, int inY)
{
	if ((inFragment.mColour & EColour_Alpha0) == 0)
	{
		Set(inX, inY, inFragment);
	}
}

void RenderTarget::Write(const std::string& inString, int inX, int inY)
{
	Write(inString, ETextGrey, inX, inY);
}

void RenderTarget::Write(const std::string& inString, EColour inColour, int inX, int inY)
{
	int x = inX;
	int y = inY;

	for (size_t i = 0; i < inString.size(); ++i)
	{
		if (inString[i] == '\n')
		{
			x = inX;
			y++;
		}
		else
		{
			Write(Fragment(inString[i], inColour), x, y);
			x++;
		}
	}
}

void RenderTarget::Write(const AsciiMesh& inMesh, int inX, int inY)
{
	inMesh.ForEachFrag( [&] (int inFragX, int inFragY, const Fragment& inFrag)
	{
		int x = inFragX + inX;
		int y = inFragY + inY;
		Write(inFrag, x, y);
	} );
}

void RenderTarget::Write(const RenderTarget& inRenderTarget, int inX, int inY)
{
	inRenderTarget.ForEach( [&] (int inFragX, int inFragY, const Fragment& inFrag)
	{
		int x = inFragX + inX;
		int y = inFragY + inY;
		Write(inFrag, x, y);
	} );
}
