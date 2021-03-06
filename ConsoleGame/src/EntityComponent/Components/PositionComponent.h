#pragma once

#include <Maths/IVec2.h>

class PositionComponent
{
public:
	PositionComponent()		{ }
	PositionComponent(int inX, int inY)					: mPosition( inX, inY ),		mPrevPosition( inX, inY )				{ }
	PositionComponent(const IVec2& inPosition)			: mPosition( inPosition ),		mPrevPosition( inPosition )				{ }
	PositionComponent(PositionComponent&& inRHS)		: mPosition( inRHS.mPosition ), mPrevPosition( inRHS.mPrevPosition )	{ }
	PositionComponent(const PositionComponent& inRHS)	: mPosition( inRHS.mPosition ), mPrevPosition( inRHS.mPrevPosition )	{ }
	~PositionComponent()	{ }

	const IVec2&	GetPosition() const						{ return mPosition; }
	void			SetPosition(const IVec2& inPosition)	{ mPosition = inPosition; }

	const IVec2&	GetPreviousPosition() const				{ return mPrevPosition; }

	void			SwapPositionBuffers()					{ mPrevPosition = mPosition; }

private:
	IVec2 mPosition;
	IVec2 mPrevPosition;
};
