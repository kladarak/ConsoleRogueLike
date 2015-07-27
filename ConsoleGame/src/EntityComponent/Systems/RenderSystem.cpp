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
	
	IVec2 position	= inEntity.GetComponent<PositionComponent>()->GetPosition();
	position		-= inCameraPosition;
	auto& mesh		= renderableComp->GetMesh();

	inTarget.Write(mesh, position.mX, position.mY);
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
