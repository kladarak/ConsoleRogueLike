#include "RenderTargetWriter.h"

#include "AsciiMesh.h"

RenderTargetWriter::RenderTargetWriter(int inColumns, int inRows)
	: mRenderTarget(inColumns, inRows)
{
}

void RenderTargetWriter::Write(char inChar, int inX, int inY)
{
	mRenderTarget.Set(inX, inY, inChar);
}

void RenderTargetWriter::Write(const Fragment& inFragment, int inX, int inY)
{
	if ((inFragment.mColour & EInvisible) == 0)
	{
		mRenderTarget.Set(inX, inY, inFragment);
	}
}

void RenderTargetWriter::Write(const std::string& inString, int inX, int inY)
{
	Write(inString, ETextGrey, inX, inY);
}

void RenderTargetWriter::Write(const std::string& inString, EColour inColour, int inX, int inY)
{
	int x = inX;
	int y = inY;

	for (size_t i = 0; i < inString.size(); ++i)
	{
		if (inString[i] == '\n')
		{
			x = inX;
			y++;
		}
		else
		{
			Write(Fragment(inString[i], inColour), x, y);
			x++;
		}
	}
}

void RenderTargetWriter::Write(const AsciiMesh& inMesh, int inX, int inY)
{
	inMesh.ForEachFrag( [&] (int inFragX, int inFragY, const Fragment& inFrag)
	{
		int x = inFragX + inX;
		int y = inFragY + inY;
		Write(inFrag, x, y);
	} );
}

void RenderTargetWriter::Write(const RenderTarget& inRenderTarget, int inX, int inY)
{
	inRenderTarget.ForEach( [&] (int inFragX, int inFragY, const Fragment& inFrag)
	{
		int x = inFragX + inX;
		int y = inFragY + inY;
		Write(inFrag, x, y);
	} );
}
