#pragma once

#include <Maths/IVec2.h>

class PositionComponent
{
public:
	PositionComponent()		{ }
	PositionComponent(const IVec2& inPosition)			: mPosition( inPosition )					{ }
	PositionComponent(PositionComponent&& inRHS)		: mPosition( std::move(inRHS.mPosition) )	{ }
	PositionComponent(const PositionComponent& inRHS)	: mPosition( inRHS.mPosition )				{ }
	~PositionComponent()	{ }

	const IVec2&	GetPosition() const						{ return mPosition; }
	void			SetPosition(const IVec2& inPosition)	{ mPosition = inPosition; }

	const IVec2&	GetPreviousPosition() const				{ return mPrevPosition; }

	void			SwapPositionBuffers()					{ mPrevPosition = mPosition; }

private:
	IVec2 mPosition;
	IVec2 mPrevPosition;
};
