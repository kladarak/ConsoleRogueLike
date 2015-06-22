#pragma once

struct IVec2
{
public:
	IVec2()					: mX(0),		mY(0)		{ }
	IVec2(int inValue)		: mX(inValue),	mY(inValue)	{ }
	IVec2(int inX, int inY)	: mX(inX),		mY(inY)		{ }

	IVec2&	operator+=(const IVec2& inRHS)			{ mX += inRHS.mX; mY += inRHS.mY; return *this; }
	IVec2&	operator-=(const IVec2& inRHS)			{ mX -= inRHS.mX; mY -= inRHS.mY; return *this; }

	bool	operator==(const IVec2& inRHS) const	{ return mX == inRHS.mX && mY == inRHS.mY;		}
	bool	operator!=(const IVec2& inRHS) const	{ return !(*this == inRHS);						}

	int mX;
	int mY;

	static const IVec2& sZero() { static const IVec2 sZeroInstance(0, 0); return sZeroInstance; }
};

inline IVec2 operator+(const IVec2& inLHS, const IVec2& inRHS)	{ IVec2 copy = inLHS; copy += inRHS; return copy;		}
inline IVec2 operator-(const IVec2& inLHS, const IVec2& inRHS)	{ IVec2 copy = inLHS; copy -= inRHS; return copy;		}
inline IVec2 operator-(const IVec2& inLHS)						{ return IVec2(-inLHS.mX, -inLHS.mY);					}
inline IVec2 operator*(const IVec2& inLHS, int inScale)			{ return IVec2(inLHS.mX * inScale, inLHS.mY * inScale); }
