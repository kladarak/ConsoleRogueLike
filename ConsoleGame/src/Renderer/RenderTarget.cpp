#include "RenderTarget.h"

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
