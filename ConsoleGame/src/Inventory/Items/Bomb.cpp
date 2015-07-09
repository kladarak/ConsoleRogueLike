#include "Bomb.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <Messages/Messages.h>

namespace
{
	// 162 = 'o' with accent
	const char		kBombIcon = gCastUCharToChar(162);
	const AsciiMesh	kBombMesh( kBombIcon );
	const ItemData	kBombData( "Bombs", kBombMesh, ERequiresAmmo );
}

Bomb::Bomb()
	: ItemBase(kBombData)
{
}
	
void Bomb::SpawnAmmo(World& inWorld, const IVec2& inPosition)
{
}

//-------------------------------------------------------------------

BombPlayerBehaviour::BombPlayerBehaviour()
	: mBombCount(10)
{
}

void BombPlayerBehaviour::OnStart(Entity inPlayer)
{
}

void BombPlayerBehaviour::OnRestart(Entity inPlayer)
{
}

void BombPlayerBehaviour::Update(Entity inPlayer, float inFrameTime)
{
}

void BombPlayerBehaviour::OnFinish(Entity inPlayer)
{
}

bool BombPlayerBehaviour::IsFinished() const
{
	return true;
}
