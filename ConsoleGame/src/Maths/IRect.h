#pragma once

#include "IVec2.h"

class IRect
{
public:
	IRect()												: mX(0),	mY(0),		mWidth(0),			mHeight(0)			{ }
	IRect(int inX, int inY, int inWidth, int inHeight)	: mX(inX),	mY(inY),	mWidth(inWidth),	mHeight(inHeight)	{ }

	bool Contains(const IVec2 inPos) const
	{
		return	( (inPos.mX >= mX)
				& (inPos.mY >= mY)
				& (inPos.mX < mWidth)
				& (inPos.mY < mHeight) ) != 0;
	}
	
	int mX;
	int mY;
	int mWidth;
	int mHeight;
};