#pragma once

#include "Colour.h"

struct Fragment
{
	char	mChar;
	EColour	mColour;

	Fragment()								: mChar(' '),		mColour(ETextGrey)	{ }
	Fragment(char inChar)					: mChar(inChar),	mColour(ETextGrey)	{ }
	Fragment(char inChar, EColour inColour) : mChar(inChar),	mColour(inColour)	{ }

	bool operator==(const Fragment& inRHS) const { return mChar == inRHS.mChar && mColour == mColour; }
};
