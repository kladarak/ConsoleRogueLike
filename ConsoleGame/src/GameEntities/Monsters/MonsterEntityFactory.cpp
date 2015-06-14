#include "MonsterEntityFactory.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/DamageZoneComponent.h>
#include <EntityComponent/Components/MessageReceiverComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>
#include <EntityComponent/Components/TriggerBoxComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>
#include <EntityComponent/Systems/PositionSystem.h>
#include <EntityComponent/Systems/DamageZoneSystem.h>

#include <GameEntities/CoinEntity.h>
#include <GameEntities/HealthEntity.h>

#include <Messages/Messages.h>

namespace MonsterEntityFactory
{

static const float	kAnimationDuration			= 0.5f;
static const float	kMovementCooldownDuration	= 1.0f;

static const AsciiMesh kMonsterFrames[] =
{
	AsciiMesh( "e", 1, 1 ),
	AsciiMesh( "@", 1, 1 ),
};

static const Animation kMonsterAnimation(kMonsterFrames, sizeof(kMonsterFrames)/sizeof(AsciiMesh), kAnimationDuration, Animation::EPlaybackStyle_Loop);

class MonsterState
{
public:
	MonsterState()
	{
		ResetMovementDuration();
	}

	~MonsterState() { }

	void ResetMovementDuration()
	{
		mMovementCooldownTime = kMovementCooldownDuration + ((rand()%10) * 0.1f);
	}

	float mMovementCooldownTime;
};

static void TakeDamage(const Entity& inMonster, MessageBroadcaster& inMsgBroadcaster)
{
	auto position = inMonster.GetComponent<PositionComponent>()->GetPosition();

	Entity(inMonster).Kill();
	switch (rand() % 3)
	{
		case 0: CoinEntity::Create(*inMonster.GetWorld(), inMsgBroadcaster, position); break;
		case 1: HealthEntity::Create(*inMonster.GetWorld(), position); break;
		default: break;
	}
}

static void OnAttacked(Entity inMonster, const AttackMsg&, MessageBroadcaster& inMsgBroadcaster)
{
	TakeDamage(inMonster, inMsgBroadcaster);
}

static IVec2 GetIntendedMovement(const Entity& inThis, float inFrameTime)
{
	IVec2 movement(0, 0);

	auto state = inThis.GetComponent<MonsterState>();
	state->mMovementCooldownTime -= inFrameTime;

	if (state->mMovementCooldownTime < 0.0f)
	{
		int direction = rand() % 4;
		switch (direction)
		{
			case 0: movement.mX =  1; break;
			case 1: movement.mX = -1; break;
			case 2: movement.mY =  1; break;
			case 3: movement.mY = -1; break;
		}

		state->ResetMovementDuration();
	}

	return movement;
}

static void Update(const Entity& inThis, float inFrameTime, MessageBroadcaster& inMsgBroadcaster)
{
	// Check if in danger zone.
	// If so, take damage.
	// If not, get intended movement
	// If moving, attack in intended direction.
	// If it's not collidable, or is monster or player, then move there.

	auto posComp = inThis.GetComponent<PositionComponent>();
	auto position = posComp->GetPosition();

	{
		bool isInDamageZone = DamageZoneSystem::IsDamageZone(*inThis.GetWorld(), inThis, position);
		if (isInDamageZone)
		{
			TakeDamage(inThis, inMsgBroadcaster);
			return;
		}
	}

	IVec2 intendedMovement = GetIntendedMovement(inThis, inFrameTime);
	if (intendedMovement == IVec2(0, 0))
	{
		return;
	}

	auto newPos = position + intendedMovement;

	{
		auto attackedEntities = PositionSystem::GetListOfEntitiesAtPosition(*inThis.GetWorld(), inThis, newPos);
		for (auto entity : attackedEntities)
		{
			auto msgRecComp = entity.GetComponent<MessageReceiverComponent>();
			if (nullptr != msgRecComp)
			{
				msgRecComp->Broadcast( AttackMsg(inThis, newPos, intendedMovement) );
			}
		}
	}

	bool isValidPos = true;
	
	auto collidablesAtPosition = CollisionSystem::GetListofCollidablesAtPosition(*inThis.GetWorld(), newPos);
	for (auto& collidable : collidablesAtPosition)
	{
		if (!collidable.HasComponent<MonsterComponent>() && !collidable.HasComponent<PlayerComponent>())
		{
			isValidPos = false;
			break;
		}
	}

	if (isValidPos)
	{
		posComp->SetPosition(newPos);
	}
}

void Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPosition)
{
	auto entity = inWorld.CreateEntity();
	
	entity.AddComponent<AnimationComponent>(kMonsterAnimation);
	entity.AddComponent<CollisionComponent>()->SetCollidableAt(0, 0);
	entity.AddComponent<DamageZoneComponent>()->SetDamageZoneAt(0, 0);
	entity.AddComponent<MonsterComponent>();
	entity.AddComponent<MonsterState>();
	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<RenderableComponent>(kMonsterFrames[0]);

	entity.AddComponent<MessageReceiverComponent>()->Register<AttackMsg>(
		[entity, &inMsgBroadcaster] (const AttackMsg& inAttackMsg)
		{
			OnAttacked(entity, inAttackMsg, inMsgBroadcaster); 
		} );
	
	entity.AddComponent<ProgramComponent>()->RegisterProgram(
		[&inMsgBroadcaster] (const Entity& inThis, float inFrameTime)
		{
			Update(inThis, inFrameTime, inMsgBroadcaster); 
		} );
}

}
