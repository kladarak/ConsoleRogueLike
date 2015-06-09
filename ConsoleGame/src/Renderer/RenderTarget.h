#pragma once

#include <vector>

class RenderTarget
{
public:
	RenderTarget(int inColumns, int inRows);

	void Set(int inCol, int inRow, char inChar);
	void Render();

	bool operator==(const RenderTarget& inRHS);
	bool operator!=(const RenderTarget& inRHS) { return !(*this == inRHS); }

private:
	std::vector< std::vector<char> >	mChars;
	int									mCols;
	int									mRows;
};
