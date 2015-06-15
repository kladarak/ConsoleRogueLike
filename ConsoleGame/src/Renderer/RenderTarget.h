#pragma once

#include <vector>
#include <cstring>

class RenderTarget
{
public:
	RenderTarget(int inColumns, int inRows);

	void		Set(int inCol, int inRow, char inChar);
	std::string GetBuffer() const;

	bool		operator==(const RenderTarget& inRHS);
	bool		operator!=(const RenderTarget& inRHS) { return !(*this == inRHS); }

private:
	std::vector< std::string >	mChars;
	int							mCols;
	int							mRows;
};
