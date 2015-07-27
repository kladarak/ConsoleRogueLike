#pragma once

#include <vector>
#include <cstring>

#include <Containers/Dynamic2DVector.h>

#include "Fragment.h"

class AsciiMesh;

class RenderTarget
{
public:
	RenderTarget(int inColumns, int inRows);

	void						Set(int inCol, int inRow, char inChar);
	void						Set(int inCol, int inRow, const Fragment& inFragment);

	int							GetColCount() const						{ return mFragments.GetColCount(); }
	int							GetRowCount() const						{ return mFragments.GetRowCount(); }

	bool						operator==(const RenderTarget& inRHS);
	bool						operator!=(const RenderTarget& inRHS)	{ return !(*this == inRHS); }

	template<typename TFunctor>
	void						ForEach(const TFunctor& inFunctor) const;
	
	void						Write(char inChar, int inX, int inY);
	void						Write(const Fragment& inFragment, int inX, int inY);
	void						Write(const std::string& inString, int inX, int inY);
	void						Write(const std::string& inString, EColour inColour, int inX, int inY);
	void						Write(const AsciiMesh& inMesh, int inX, int inY);
	void						Write(const RenderTarget& inRenderTarget, int inX, int inY);

private:
	Dynamic2DVector<Fragment>	mFragments;

};

template<typename TFunctor>
void RenderTarget::ForEach(const TFunctor& inFunctor) const
{
	mFragments.ForEach(inFunctor);
}
