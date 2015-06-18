#pragma once

#include <Core/Debug/Assert.h>
#include <Maths/IVec2.h>

enum EOrientation
{
	EOrientation_FaceUp,
	EOrientation_FaceDown,
	EOrientation_FaceLeft,
	EOrientation_FaceRight,
	
	EOrientation_Count,
};

static inline IVec2 gGetOrientationVector(EOrientation inOrientation)
{
	switch (inOrientation)
	{
		case EOrientation_FaceUp:		return IVec2( 0, -1);
		case EOrientation_FaceDown:		return IVec2( 0,  1);
		case EOrientation_FaceLeft:		return IVec2(-1,  0);
		case EOrientation_FaceRight:	return IVec2( 1,  0);
		default: assert(false);			return IVec2::sZero();
	}
}
