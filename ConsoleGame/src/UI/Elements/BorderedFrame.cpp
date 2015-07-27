#include "BorderedFrame.h"

namespace
{
	const Fragment kVerticalWall		( gCastUCharToChar(179), ETextWhite );
	const Fragment kHorizontalWall		( gCastUCharToChar(196), ETextWhite );

	const Fragment kTopLeftCorner		( gCastUCharToChar(218), ETextWhite );
	const Fragment kTopRightCorner		( gCastUCharToChar(191), ETextWhite );
	const Fragment kBottomLeftCorner	( gCastUCharToChar(192), ETextWhite );
	const Fragment kBottomRightCorner	( gCastUCharToChar(217), ETextWhite );
}

BorderedFrame::BorderedFrame(const IVec2& inSize)
	: mSize(inSize)
{
}

BorderedFrame::BorderedFrame(int inWidth, int inHeight)
	: mSize(inWidth, inHeight)
{
}

RenderTarget BorderedFrame::Render() const
{
	RenderTarget target(mSize);
	
	for (int i = 1; i < mSize.mX-1; ++i) { target.Write(kHorizontalWall, i,			0);				}
	for (int i = 1; i < mSize.mX-1; ++i) { target.Write(kHorizontalWall, i,			mSize.mY-1);	}
	for (int i = 1; i < mSize.mY-1; ++i) { target.Write(kVerticalWall,	0,			i);				}
	for (int i = 1; i < mSize.mY-1; ++i) { target.Write(kVerticalWall,	mSize.mX-1, i);				}
		
	target.Write( kTopLeftCorner,		0,			0			);
	target.Write( kTopRightCorner,		mSize.mX-1,	0			);
	target.Write( kBottomLeftCorner,	0,			mSize.mY-1	);
	target.Write( kBottomRightCorner,	mSize.mX-1,	mSize.mY-1	);

	return target;
}
