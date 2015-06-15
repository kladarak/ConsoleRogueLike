#include "RenderTarget.h"

RenderTarget::RenderTarget(int inColumns, int inRows)
	: mChars(inRows, std::string(inColumns, ' ') )
	, mCols(inColumns)
	, mRows(inRows)
{
}

void RenderTarget::Set(int inCol, int inRow, char inChar)
{
	if (inCol >= 0 && inCol < mCols && inRow >= 0 && inRow < mRows)
	{
		mChars[inRow][inCol] = inChar;
	}
}

bool RenderTarget::operator==(const RenderTarget& inRHS)
{
	if (mCols != inRHS.mCols) return false;
	if (mRows != inRHS.mRows) return false;

	for (int row = 0; row < mRows; ++row)
	{
		for (int col = 0; col < mCols; ++col)
		{
			if (mChars[row][col] != inRHS.mChars[row][col])
			{
				return false;
			}
		}
	}

	return true;
}

std::string RenderTarget::GetBuffer() const
{
	std::string buffer;

	for (auto& row : mChars)
	{
		buffer += row;
		buffer += "\n";
	}
	
	return buffer;
}
