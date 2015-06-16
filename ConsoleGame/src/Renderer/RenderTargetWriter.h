#pragma once

#include "RenderTarget.h"

class AsciiMesh;

class RenderTargetWriter
{
public:
	RenderTargetWriter(int inColumns, int inRows);
	
	void				Write(char inChar, int inX, int inY);
	void				Write(const std::string& inString, int inX, int inY);
	void				Write(const AsciiMesh& inMesh, int inX, int inY);

	std::string			GetRenderBuffer() const { return mRenderTarget.GetBuffer(); }

private:
	RenderTarget		mRenderTarget;

};
