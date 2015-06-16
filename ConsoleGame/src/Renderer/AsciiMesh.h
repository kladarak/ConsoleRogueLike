#pragma once

#include <Containers/Dynamic2DVectorWithOffset.h>
#include <Maths/IVec2.h>
#include <Maths/IRect.h>

class AsciiMesh : public Dynamic2DVectorWithOffset<char>
{
public:
	AsciiMesh()							: Dynamic2DVectorWithOffset<char>()					{ }
	AsciiMesh(AsciiMesh&& inRHS)		: Dynamic2DVectorWithOffset<char>( inRHS )			{ }
	AsciiMesh(const AsciiMesh& inRHS)	: Dynamic2DVectorWithOffset<char>( inRHS )			{ }
	AsciiMesh(char inChar)				: Dynamic2DVectorWithOffset<char>( 1, 1, inChar )	{ }
	AsciiMesh(const char* inChars, size_t inCols, size_t inRows, const IVec2& inCentreOffset = IVec2(0, 0));

	void							SetCharAtPosition(int inX, int inY, char inChar)	{ Set(inX, inY, inChar); }

	template<typename TFunctor>
	void							ForEachFrag(const TFunctor& inFunctor) const		{ ForEach(inFunctor); }

};
