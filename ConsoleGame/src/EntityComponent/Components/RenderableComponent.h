#pragma once

#include <Renderer/AsciiMesh.h>

class Entity;
class RenderTarget;

class RenderableComponent
{
public:
	RenderableComponent()	{ }
	~RenderableComponent()	{ }
	RenderableComponent(RenderableComponent&& inRHS)		: mMesh( std::move(inRHS.mMesh) )	{ }
	RenderableComponent(const RenderableComponent& inRHS)	: mMesh( inRHS.mMesh )				{ }

	void				SetMesh(const AsciiMesh& inMesh)	{ mMesh = inMesh;	}
	const AsciiMesh&	GetMesh() const						{ return mMesh;		}

private:
	AsciiMesh mMesh;

};
