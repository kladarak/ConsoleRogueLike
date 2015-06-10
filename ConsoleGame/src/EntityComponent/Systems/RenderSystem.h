#pragma once

class World;
class RenderTarget;
struct IVec2;

namespace RenderSystem
{
	void Render(World& inWorld, const IVec2& inCameraPosition, RenderTarget& inTarget);
}
