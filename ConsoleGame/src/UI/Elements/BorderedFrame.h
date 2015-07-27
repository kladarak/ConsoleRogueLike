#pragma once

#include <Maths/IVec2.h>
#include <Renderer/RenderTarget.h>

class BorderedFrame
{
public:
	BorderedFrame(const IVec2& inSize);
	BorderedFrame(int inWidth, int inHeight);

	RenderTarget	Render() const;

private:
	IVec2			mSize;
};
