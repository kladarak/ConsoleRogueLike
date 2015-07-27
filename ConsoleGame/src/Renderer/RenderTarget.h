#pragma once

#include <vector>
#include <cstring>

#include <Containers/Dynamic2DVector.h>

#include "Fragment.h"

class AsciiMesh;
struct IVec2;

class RenderTarget
{
public:
	RenderTarget(const IVec2& inSize);
	RenderTarget(int inColumns, int inRows);

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
