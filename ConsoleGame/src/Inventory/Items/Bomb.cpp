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

#include "EAttackStrength.h"

namespace
{
	// 162 = 'o' with accent
	const char		kBombIcon = gCastUCharToChar(162);
	const AsciiMesh	kBombMesh( kBombIcon );
	const ItemData	kBombData( "Bombs", kBombMesh, ERequiresAmmo );

	const float		kCooldownTime	= 0.5f;
	const float		kBombTickTime	= 2.0f;

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
		
		static const bool kSmokeMask[] =
		{
			false,	false,	true,	false,	false,
			false,	true,	true,	true,	false,
			true,	true,	true,	true,	true,
			false,	true,	true,	true,	false,
			false,	false,	true,	false,	false,
		};

		static const IVec2	kMeshCentreOffset(-2, -2);
		static const int	kMeshWidth = 5;
		static const int	kMeshHeight = 5;

		static const char kSmokeThick	= gCastUCharToChar(178);
		static const char kSmokeNormal	= gCastUCharToChar(177);
		static const char kSmokeThin	= gCastUCharToChar(176);
		
		static AsciiMesh sGenerateMesh(char inChar)
		{
			char smoke[kMeshWidth * kMeshHeight];

			for (int i = 0; i < kMeshWidth * kMeshHeight; ++i)
			{
				smoke[i] = kSmokeMask[i] ? inChar : ' ';
			}

			return AsciiMesh(smoke, kMeshWidth, kMeshHeight, kMeshCentreOffset);
		}

		static const AsciiMesh kSmokeThickMesh	= sGenerateMesh(kSmokeThick);
		static const AsciiMesh kSmokeNormalMesh	= sGenerateMesh(kSmokeNormal);
		static const AsciiMesh kSmokeThinMesh	= sGenerateMesh(kSmokeThin);

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

		static const EAttackStrength kDamageMask[] =
		{
			EAttackStrength_None,		EAttackStrength_None,		EAttackStrength_Weak,		EAttackStrength_None,		EAttackStrength_None,
			EAttackStrength_None,		EAttackStrength_Weak,		EAttackStrength_Normal,		EAttackStrength_Weak,		EAttackStrength_None,
			EAttackStrength_Weak,		EAttackStrength_Normal,		EAttackStrength_Strong,		EAttackStrength_Normal,		EAttackStrength_Weak,
			EAttackStrength_None,		EAttackStrength_Weak,		EAttackStrength_Normal,		EAttackStrength_Weak,		EAttackStrength_None,
			EAttackStrength_None,		EAttackStrength_None,		EAttackStrength_Weak,		EAttackStrength_None,		EAttackStrength_None,
		};
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
		
		auto bombPos = inThis.GetComponent<PositionComponent>()->GetPosition();

		for (int row = 0; row < ExplosionAnimation::kMeshHeight; ++row)
		{
			for (int col = 0; col < ExplosionAnimation::kMeshWidth; ++col)
			{
				IVec2			offset		= IVec2(col, row) + ExplosionAnimation::kMeshCentreOffset;
				IVec2			position	= bombPos + offset;
				EAttackStrength	atkStrength = ExplosionAnimation::kDamageMask[(row * ExplosionAnimation::kMeshWidth) + col];


				AttackMsg	attackMsg(inThis, position, offset, AttackMsg::EEffect_PushBack, atkStrength);
				MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, position, attackMsg);
			}
		}
	}
	else if (mTimeRemaining < -ExplosionAnimation::kAnimationDuration)
	{
		inThis.Kill();
	}
}
