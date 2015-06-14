#pragma once

#include <Containers/Dynamic2DVector.h>
#include <Maths/IVec2.h>

class AsciiMesh
{
public:
	AsciiMesh() { }
	AsciiMesh(AsciiMesh&& inRHS)		: mFragments( std::move(inRHS.mFragments) ),	mCentreOffset( inRHS.mCentreOffset ) { }
	AsciiMesh(const AsciiMesh& inRHS)	: mFragments( inRHS.mFragments ),				mCentreOffset( inRHS.mCentreOffset ) { }
	AsciiMesh(const char* inChars, size_t inCols, size_t inRows, const IVec2& inCentreOffset = IVec2(0, 0));
	AsciiMesh(char inChar);

	void							SetCharAtPosition(int inX, int inY, char inChar)	{ mFragments.Set(inX, inY, inChar); }
	const Dynamic2DVector<char>&	GetFragments() const								{ return mFragments; }

	void							SetCentreOffset(const IVec2& inOffset)				{ mCentreOffset = inOffset; }
	const IVec2&					GetCentreOffset() const								{ return mCentreOffset; }

	template<typename TFunctor>
	void		ForEachFrag(const TFunctor& inFunctor) const;

private:
	Dynamic2DVector<char>			mFragments;
	IVec2							mCentreOffset;
};


template<typename TFunctor>
void AsciiMesh::ForEachFrag(const TFunctor& inFunctor) const
{
	mFragments.ForEach( [&] (int inX, int inY, char inChar)
	{
		int x = inX + mCentreOffset.mX;
		int y = inY + mCentreOffset.mY;
		inFunctor(x, y, inChar);
	} );
}
