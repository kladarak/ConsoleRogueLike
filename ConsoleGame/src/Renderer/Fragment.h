#pragma once

#include "Colour.h"

static inline char gCastUCharToChar(unsigned char inChar) { return -128+(inChar%128); }

struct Fragment
{
	char	mChar;
	EColour	mColour;

	Fragment()								: mChar(' '),	 mColour(ETextGrey)	{ }
	Fragment(char inChar)					: mChar(inChar), mColour(ETextGrey)	{ }
	Fragment(char inChar, EColour inColour)	: mChar(inChar), mColour(inColour)	{ }

	bool operator==(const Fragment& inRHS) const { return mChar == inRHS.mChar && mColour == mColour; }
};
