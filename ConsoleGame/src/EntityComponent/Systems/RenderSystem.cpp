#include "RenderSystem.h"

#include <Renderer/RenderTarget.h>

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityFilter.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

namespace RenderSystem
{
	
static void RenderEntityToTarget(const Entity& inEntity, const IVec2& inCameraPosition, RenderTarget& inTarget)
{
	auto renderableComp = inEntity.GetComponent<RenderableComponent>();
	if (!renderableComp->IsVisible())
	{
		return;
	}
	
	auto positionComp	= inEntity.GetComponent<PositionComponent>();
	IVec2 position	= positionComp->GetPosition();
	position		-= inCameraPosition;

	auto& mesh		= renderableComp->GetMesh();

	mesh.ForEachFrag( [&] (int inX, int inY, const Fragment& inFrag)
	{
		if ((inFrag.mColour & EColour_Alpha0) == 0)
		{
			int x = inX + position.mX;
			int y = inY + position.mY;
			inTarget.Set(x, y, inFrag);
		}
	} );
}

void Render(World& inWorld, const IVec2& inCameraPosition, RenderTarget& inTarget)
{
	auto renderables = inWorld.GetEntities( EntityFilter().MustHave<PositionComponent>().MustHave<RenderableComponent>() );

	for (auto& renderable : renderables)
	{
		RenderEntityToTarget(renderable, inCameraPosition, inTarget);
	}
}

}
