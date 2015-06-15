#include "RenderTargetWriter.h"

#include "AsciiMesh.h"

RenderTargetWriter::RenderTargetWriter(int inColumns, int inRows)
	: mRenderTarget(inColumns, inRows)
{
}

void RenderTargetWriter::Write(const std::string& inString, int inX, int inY)
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
			mRenderTarget.Set(x, inY, inString[i]);
			x++;
		}
	}
}

void RenderTargetWriter::Write(const AsciiMesh& inMesh, int inX, int inY)
{
	inMesh.ForEachFrag( [&] (int inFragX, int inFragY, char inChar)
	{
		int x = inFragX + inX;
		int y = inFragY + inY;
		mRenderTarget.Set(x, y, inChar);
	} );
}
