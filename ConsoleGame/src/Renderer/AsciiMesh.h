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

	void							SetCharAtPosition(const IVec2& inPosition, char inChar)	{ Fragment frag = { inPosition, inChar }; mFragments.push_back(frag); }
	const std::vector<Fragment>&	GetFragments() const									{ return mFragments; }

private:
	std::vector<Fragment>			mFragments;
};
