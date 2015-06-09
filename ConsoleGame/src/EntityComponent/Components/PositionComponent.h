#pragma once

#include <Maths/IVec2.h>

class PositionComponent
{
public:
	PositionComponent()		{ }
	~PositionComponent()	{ }
	PositionComponent(PositionComponent&& inRHS)		: mPosition( std::move(inRHS.mPosition) )	{ }
	PositionComponent(const PositionComponent& inRHS)	: mPosition( inRHS.mPosition )				{ }

	const IVec2&	GetPosition() const						{ return mPosition; }
	void			SetPosition(const IVec2& inPosition)	{ mPosition = inPosition; }

private:
	IVec2 mPosition;

};
