#include "Bomb.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/PlayerComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
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

	const float		kCooldownTime	= 0.5f;
	const float		kBombTickTime	= 2.0f;
	const int		kAttackStrength	= 1;

	namespace FlashAnimation
	{
		static const float kKFTime = 0.125f;

		static const AsciiMesh kKeyFrames[] =
		{
			AsciiMesh( kBombIcon ),
			AsciiMesh( ' ' ),
		};

		static const Animation kAnimation(	kKeyFrames, 
											gElemCount(kKeyFrames), 
											kKFTime, 
											Animation::EPlaybackStyle_Loop );
	}

	namespace ExplosionAnimation
	{
		static const float kKFTime = 0.125f;
		
		static const char kSmokeThick	= gCastUCharToChar(178);
		static const char kSmokeNormal	= gCastUCharToChar(177);
		static const char kSmokeThin	= gCastUCharToChar(176);
		
		static const AsciiMesh kSmokeThickMesh	(kSmokeThick,	3, 3, IVec2(-1, -1));
		static const AsciiMesh kSmokeNormalMesh	(kSmokeNormal,	3, 3, IVec2(-1, -1));
		static const AsciiMesh kSmokeThinMesh	(kSmokeThin,	3, 3, IVec2(-1, -1));

		static const AsciiMesh kKeyFrames[] =
		{
			kSmokeThickMesh,
			kSmokeNormalMesh,
			kSmokeThickMesh,
			kSmokeNormalMesh,
			kSmokeThinMesh,
			kSmokeNormalMesh,
			kSmokeThinMesh,
		};

		static const Animation kAnimation(	kKeyFrames, 
											gElemCount(kKeyFrames), 
											kKFTime, 
											Animation::EPlaybackStyle_Once );

		static const float kAnimationDuration = gElemCount(kKeyFrames) * kKFTime;
	}
}

//-------------------------------------------------------------------------------------------------

class BombTimer
{
public:
	BombTimer() : mTimeRemaining(kBombTickTime) { }
	~BombTimer()								{ }

	void Update(Entity inThis, float inFrameTime);

private:
	float mTimeRemaining;
};

//-------------------------------------------------------------------------------------------------

Bomb::Bomb()
	: ItemBase(kBombData)
{
}
	
void Bomb::SpawnAmmo(World& inWorld, const IVec2& inPosition)
{
	auto bomb = EntityBuilder(inWorld)
				.AddComponent<PositionComponent>(inPosition)
				.AddComponent<RenderableComponent>(kBombMesh)
				.AddComponent<TriggerBoxComponent>()
				.Create();

	bomb.GetComponent<TriggerBoxComponent>()->RegisterOnEnterCallback
	( 
		[&] (Entity inThis, Entity inPlayer)
		{
			if (inPlayer.HasComponent<PlayerComponent>())
			{
				mBehaviour.IncBombCount();
				inThis.Kill();
			}
		}
	);
}

//-------------------------------------------------------------------------------------------------

BombPlayerBehaviour::BombPlayerBehaviour()
	: mBombCount(10)
	, mTimeElapsedSinceStart(0.0f)
{
}

void BombPlayerBehaviour::OnStart(Entity inPlayer)
{
	if (mBombCount > 0)
	{
		auto playerPos = inPlayer.GetComponent<PositionComponent>()->GetPosition();

		auto bomb = EntityBuilder(*inPlayer.GetWorld())
						.AddComponent<PositionComponent>(playerPos)
						.AddComponent<ProgramComponent>()
						.AddComponent<RenderableComponent>(kBombMesh)
						.AddComponent<AnimationComponent>( FlashAnimation::kAnimation )
						.AddComponent<BombTimer>()
						.Create();

		bomb.GetComponent<ProgramComponent>()->RegisterProgram ( [] (Entity inThis, float inFrameTime) 
		{
			inThis.GetComponent<BombTimer>()->Update(inThis, inFrameTime);
		} );

		--mBombCount;
	}

	mTimeElapsedSinceStart = kCooldownTime;
}

void BombPlayerBehaviour::OnRestart(Entity inPlayer)
{
	if (IsFinished())
	{
		OnStart(inPlayer);
	}
}

void BombPlayerBehaviour::Update(Entity /*inPlayer*/, float inFrameTime)
{
	mTimeElapsedSinceStart -= inFrameTime;
}

void BombPlayerBehaviour::OnFinish(Entity /*inPlayer*/)
{
}

bool BombPlayerBehaviour::IsFinished() const
{
	return mTimeElapsedSinceStart <= 0.0f;
}

//-------------------------------------------------------------------------------------------------

void BombTimer::Update(Entity inThis, float inFrameTime)
{
	bool wasTicking = mTimeRemaining > 0.0f;
	mTimeRemaining -= inFrameTime;

	if (wasTicking && mTimeRemaining <= 0.0f)
	{
		inThis.GetComponent<AnimationComponent>()->SetAnimation( ExplosionAnimation::kAnimation );
	}
				
	if (mTimeRemaining <= 0.0f)
	{
		if (mTimeRemaining >= -ExplosionAnimation::kAnimationDuration)
		{
			auto bombPos = inThis.GetComponent<PositionComponent>()->GetPosition();

			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					IVec2		offset(i, j);
					IVec2		position = bombPos + offset;
					AttackMsg	attackMsg(inThis, position, offset, AttackMsg::EEffect_PushBack, kAttackStrength);
					MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, position, attackMsg);
				}
			}
		}
		else
		{
			inThis.Kill();
		}
	}
}
