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
	auto positionComp	= inEntity.GetComponent<PositionComponent>();
	auto renderableComp = inEntity.GetComponent<RenderableComponent>();

	IVec2 position	= positionComp->GetPosition();
	position		-= inCameraPosition;

	auto& mesh		= renderableComp->GetMesh();
	auto& fragments = mesh.GetFragments();

	for (auto& frag : fragments)
	{
		auto fragPos = frag.mPosition + position;
		inTarget.Set(fragPos.mX, fragPos.mY, frag.mCharacter);
	}
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
