#pragma once

#include <Containers/Dynamic2DVectorWithOffset.h>
#include "Colour.h"

struct Fragment
{
	char	mChar;
	EColour	mColour;

	Fragment()								: mChar(' '),		mColour(ETextGrey)	{ }
	Fragment(char inChar)					: mChar(inChar),	mColour(ETextGrey)	{ }
	Fragment(char inChar, EColour inColour) : mChar(inChar),	mColour(inColour)	{ }
};

class AsciiMesh : public Dynamic2DVectorWithOffset<Fragment>
{
private:
	typedef Dynamic2DVectorWithOffset<Fragment> Base;

public:
	AsciiMesh()							: Base()				{ }
	AsciiMesh(AsciiMesh&& inRHS)		: Base( inRHS )			{ }
	AsciiMesh(const AsciiMesh& inRHS)	: Base( inRHS )			{ }
	AsciiMesh(char inChar)				: Base( 1, 1, inChar )	{ }
	AsciiMesh(const char* inChars, size_t inCols, size_t inRows, const IVec2& inCentreOffset = IVec2(0, 0));
	AsciiMesh(char inClearChar, size_t inCols, size_t inRows, const IVec2& inCentreOffset = IVec2(0, 0)) : Base( inCols, inRows, inClearChar, inCentreOffset ) { }

	void SetCharAtPosition(int inX, int inY, char inChar)	{ Set(inX, inY, inChar); }

	template<typename TFunctor>
	void ForEachFrag(const TFunctor& inFunctor) const		{ ForEach(inFunctor); }

};

static inline char gCastUCharToChar(unsigned char inChar)	{ return -128+(inChar%128); }
