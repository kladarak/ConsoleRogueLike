#pragma once

#include <Renderer/AsciiMesh.h>

class Entity;
class RenderTarget;

class RenderableComponent
{
public:
	RenderableComponent()									: mVisible(true)												{ }
	RenderableComponent(const AsciiMesh& inMesh)			: mMesh(inMesh),					mVisible(true)				{ }
	RenderableComponent(RenderableComponent&& inRHS)		: mMesh( std::move(inRHS.mMesh) ),	mVisible(inRHS.mVisible)	{ }
	RenderableComponent(const RenderableComponent& inRHS)	: mMesh( inRHS.mMesh ),				mVisible(inRHS.mVisible)	{ }
	~RenderableComponent()	{ }

	void				SetMesh(const AsciiMesh& inMesh)	{ mMesh = inMesh;	}
	const AsciiMesh&	GetMesh() const						{ return mMesh;		}

	void				SetVisible(bool inVisible)			{ mVisible = inVisible; }
	bool				IsVisible() const					{ return mVisible;		}

private:
	AsciiMesh			mMesh;
	bool				mVisible;
};
