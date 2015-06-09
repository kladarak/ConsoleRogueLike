#include "RenderSystem.h"

#include <Renderer/RenderTarget.h>

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

namespace RenderSystem
{
	
static void RenderEntityToTarget(const Entity& inEntity, RenderTarget& inTarget)
{
	auto positionComp	= inEntity.GetComponent<PositionComponent>();
	auto renderableComp = inEntity.GetComponent<RenderableComponent>();

	if (!positionComp && !renderableComp)
	{
		return;
	}

	IVec2 position	= positionComp->GetPosition();
	auto& mesh		= renderableComp->GetMesh();
	auto& fragments = mesh.GetFragments();

	for (auto& frag : fragments)
	{
		auto fragPos = frag.mPosition + position;
		inTarget.Set(fragPos.mX, fragPos.mY, frag.mCharacter);
	}
}

void Render(World* inWorld, RenderTarget& inTarget)
{
	inWorld->ForEachEntity( [&] (Entity inEntity)
	{
		RenderEntityToTarget(inEntity, inTarget);
	} );
}

}
