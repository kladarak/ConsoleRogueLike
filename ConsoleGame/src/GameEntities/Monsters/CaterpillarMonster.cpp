#include "CaterpillarMonster.h"

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>

#include <Containers/ContainerMacros.h>

#include "MonsterBuilder.h"

namespace CaterpillarMonster
{

static const float kKFDuration = 0.1f;

namespace Animations
{
	namespace MovementLeft
	{
		static const Fragment kKF1Fragments[] = 
		{
			Fragment('o', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen),
		};
		static const AsciiMesh kKF1In(kKF1Fragments, 4, 1, IVec2(0, 0));
		static const AsciiMesh kKF1Out(kKF1Fragments, 4, 1, IVec2(-2, 0));

		static const Fragment kKF2Fragments[] = 
		{
			Fragment(' ', EInvisible), Fragment('/', ETextGreen), Fragment('\\', ETextGreen),	Fragment(' ', EInvisible),
			Fragment('o', ETextGreen), Fragment(' ', EInvisible), Fragment(' ', EInvisible),	Fragment('\\', ETextGreen),
		};
		static const AsciiMesh kKF2In(kKF2Fragments, 4, 2, IVec2(0, -1));
		static const AsciiMesh kKF2Out(kKF2Fragments, 4, 2, IVec2(-2, -1));

		static const Fragment kKF3Fragments[] = 
		{
			Fragment(' ', EInvisible), Fragment('_', ETextGreen), Fragment(' ', EInvisible),
			Fragment('/', ETextGreen), Fragment(' ', EInvisible), Fragment('\\', ETextGreen),
			Fragment('o', ETextGreen), Fragment(' ', EInvisible), Fragment('|', ETextGreen),
		};
		static const AsciiMesh kKF3In(kKF3Fragments, 3, 3, IVec2(0, -2));
		static const AsciiMesh kKF3Out(kKF3Fragments, 3, 3, IVec2(-1, -2));

		static const Fragment kKF4Fragments[] = 
		{
			Fragment('/', ETextGreen), Fragment('\\', ETextGreen),
			Fragment('|', ETextGreen), Fragment('|', ETextGreen),
			Fragment('o', ETextGreen), Fragment('|', ETextGreen),
		};
		static const AsciiMesh kKF4(kKF4Fragments, 2, 3, IVec2(0, -2));

		static const AsciiMesh kKeyFrames[] =
		{
			kKF1In,
			kKF2In,
			kKF3In,
			kKF4,
			kKF4,
			kKF3Out,
			kKF2Out,
			kKF1Out,
		};

		static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFDuration, Animation::EPlaybackStyle_Once);
	}

	namespace MovementRight
	{
		static const Fragment kKF1Fragments[] = 
		{
			Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('o', ETextGreen),
		};
		static const AsciiMesh kKF1In(kKF1Fragments, 4, 1, IVec2(-3, 0));
		static const AsciiMesh kKF1Out(kKF1Fragments, 4, 1, IVec2(-1, 0));

		static const Fragment kKF2Fragments[] = 
		{
			Fragment(' ', EInvisible), Fragment('/', ETextGreen), Fragment('\\', ETextGreen),	Fragment(' ', EInvisible),
			Fragment('/', ETextGreen), Fragment(' ', EInvisible), Fragment(' ', EInvisible),	Fragment('o', ETextGreen),
		};
		static const AsciiMesh kKF2In(kKF2Fragments, 4, 2, IVec2(-3, -1));
		static const AsciiMesh kKF2Out(kKF2Fragments, 4, 2, IVec2(-1, -1));

		static const Fragment kKF3Fragments[] = 
		{
			Fragment(' ', EInvisible), Fragment('_', ETextGreen), Fragment(' ', EInvisible),
			Fragment('/', ETextGreen), Fragment(' ', EInvisible), Fragment('\\', ETextGreen),
			Fragment('|', ETextGreen), Fragment(' ', EInvisible), Fragment('o', ETextGreen),
		};
		static const AsciiMesh kKF3In(kKF3Fragments, 3, 3, IVec2(-2, -2));
		static const AsciiMesh kKF3Out(kKF3Fragments, 3, 3, IVec2(-1, -2));

		static const Fragment kKF4Fragments[] = 
		{
			Fragment('/', ETextGreen), Fragment('\\', ETextGreen),
			Fragment('|', ETextGreen), Fragment('|', ETextGreen),
			Fragment('|', ETextGreen), Fragment('o', ETextGreen),
		};
		static const AsciiMesh kKF4(kKF4Fragments, 2, 3, IVec2(-1, -2));

		static const AsciiMesh kKeyFrames[] =
		{
			kKF1In,
			kKF2In,
			kKF3In,
			kKF4,
			kKF4,
			kKF3Out,
			kKF2Out,
			kKF1Out,
		};

		static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFDuration, Animation::EPlaybackStyle_Once);
	}

	namespace MovementTurnLeft
	{
		static const AsciiMesh kKeyFrames[] =
		{
			MovementRight::kKF1In,
			MovementRight::kKF2In,
			MovementRight::kKF3In,
			MovementRight::kKF4,
			MovementLeft::kKF4,
			MovementLeft::kKF3Out,
			MovementLeft::kKF2Out,
			MovementLeft::kKF1Out,
		};

		static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFDuration, Animation::EPlaybackStyle_Once);
	}

	namespace MovementTurnRight
	{
		static const AsciiMesh kKeyFrames[] =
		{
			MovementLeft::kKF1In,
			MovementLeft::kKF2In,
			MovementLeft::kKF3In,
			MovementLeft::kKF4,
			MovementRight::kKF4,
			MovementRight::kKF3Out,
			MovementRight::kKF2Out,
			MovementRight::kKF1Out,
		};

		static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFDuration, Animation::EPlaybackStyle_Once);
	}

	namespace IdleFaceLeft
	{
		static const Fragment kKF1Fragments[] = 
		{
			Fragment('o', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('/', ETextGreen),
		};
		static const AsciiMesh kKF1(kKF1Fragments, 4, 1, IVec2(0, 0));

		static const Fragment kKF2Fragments[] = 
		{
			Fragment('o', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen),
		};
		static const AsciiMesh kKF2(kKF2Fragments, 4, 1, IVec2(0, 0));

		static const Fragment kKF3Fragments[] = 
		{
			Fragment('o', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('\\', ETextGreen),
		};
		static const AsciiMesh kKF3(kKF3Fragments, 4, 1, IVec2(0, 0));
		
		static const AsciiMesh kKeyFrames[] =
		{
			kKF1,
			kKF2,
			kKF3,
		};

		static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFDuration, Animation::EPlaybackStyle_Loop);
	}

	namespace IdleFaceRight
	{
		static const Fragment kKF1Fragments[] = 
		{
			Fragment('\\', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('o', ETextGreen),
		};
		static const AsciiMesh kKF1(kKF1Fragments, 4, 1, IVec2(-3, 0));

		static const Fragment kKF2Fragments[] = 
		{
			Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('o', ETextGreen),
		};
		static const AsciiMesh kKF2(kKF2Fragments, 4, 1, IVec2(-3, 0));

		static const Fragment kKF3Fragments[] = 
		{
			Fragment('/', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('o', ETextGreen),
		};
		static const AsciiMesh kKF3(kKF3Fragments, 4, 1, IVec2(-3, 0));
		
		static const AsciiMesh kKeyFrames[] =
		{
			kKF1,
			kKF2,
			kKF3,
		};

		static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFDuration, Animation::EPlaybackStyle_Loop);
	}
}

class CaterpillarMonsterComponent
{
public:
	CaterpillarMonsterComponent() : mDirection(ELeft), mMovemntCycleCount(0) { }
	~CaterpillarMonsterComponent() { }

	void Update(Entity inThis, float inFrameTime); 

private:

	enum EMovementDirection
	{
		ELeft,
		ERight,
	};

	EMovementDirection	mDirection;
	int					mMovemntCycleCount;

};

void CaterpillarMonsterComponent::Update(Entity inThis, float /*inFrameTime*/)
{
	auto animComp = inThis.GetComponent<AnimationComponent>();

	if (animComp->IsSelectedAnimationFinished())
	{
		auto posComp = inThis.GetComponent<PositionComponent>();
		auto position = posComp->GetPosition();
		position.mX += (mDirection == ELeft) ? -2 : 2;
		posComp->SetPosition(position);

		++mMovemntCycleCount;

		const Animation& nextAnimation = [&]
		{
			if (mMovemntCycleCount > 3)
			{
				mMovemntCycleCount = 0;
				switch (mDirection)
				{
					case ELeft:
						mDirection = ERight;
						return Animations::MovementTurnRight::kAnimation;

					case ERight:
						mDirection = ELeft;
						return Animations::MovementTurnLeft::kAnimation;
				}
			}
			else
			{
				switch (mDirection)
				{
					case ELeft:		return Animations::MovementLeft::kAnimation;
					case ERight:	return Animations::MovementRight::kAnimation;
				}
			}

			return Animations::Idle::kAnimation;
		} ();

		animComp->SetAnimation( nextAnimation );
	}
}

Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPos)
{
	auto entity = MonsterBuilder(inWorld, &inMsgBroadcaster)
					.SetAnimation( Animations::MovementLeft::kAnimation )
					.SetRenderable( Animations::MovementLeft::kKeyFrames[0] )
					.SetPosition(inPos)
					.Create();

	entity.AddComponent<CaterpillarMonsterComponent>();

	entity.GetComponent<ProgramComponent>()->RegisterProgram
	(
		[&inMsgBroadcaster] (const Entity& inThis, float inFrameTime)
		{
			inThis.GetComponent<CaterpillarMonsterComponent>()->Update(inThis, inFrameTime); 
		} 
	);

	return entity;
}

}
