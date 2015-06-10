#pragma once

#include <vector>
#include <Maths/IVec2.h>

class AsciiMesh
{
public:
	struct Fragment
	{
		IVec2	mPosition;
		char	mCharacter;
	};

	AsciiMesh() { }
	AsciiMesh(AsciiMesh&& inRHS)						: mFragments( std::move(inRHS.mFragments) ) { }
	AsciiMesh(const AsciiMesh& inRHS)					: mFragments( inRHS.mFragments )			{ }
	AsciiMesh(const std::vector<Fragment>& inFragments) : mFragments( inFragments )					{ }

	void							SetCharAtPosition(const IVec2& inPosition, char inChar)	{ Fragment frag = { inPosition, inChar }; mFragments.push_back(frag); }
	const std::vector<Fragment>&	GetFragments() const									{ return mFragments; }

private:
	std::vector<Fragment>			mFragments;

};
