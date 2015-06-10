#pragma once

#include <vector>
#include <Maths/IVec2.h>

class AsciiMesh
{
public:
	struct Fragment
	{
		int		mX;
		int		mY;
		char	mCharacter;
	};

	AsciiMesh() { }
	AsciiMesh(AsciiMesh&& inRHS)						: mFragments( std::move(inRHS.mFragments) ) { }
	AsciiMesh(const AsciiMesh& inRHS)					: mFragments( inRHS.mFragments )			{ }
	AsciiMesh(const std::vector<Fragment>& inFragments) : mFragments( inFragments )					{ }

	void							SetCharAtPosition(int inX, int inY, char inChar)	{ mFragments.push_back( { inX, inY, inChar } ); }
	const std::vector<Fragment>&	GetFragments() const								{ return mFragments; }

private:
	std::vector<Fragment>			mFragments;

};
