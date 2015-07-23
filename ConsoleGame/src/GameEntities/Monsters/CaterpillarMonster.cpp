#include "CaterpillarMonster.h"

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/MonsterComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>

#include <EntityComponent/Systems/CollisionSystem.h>

#include <Containers/ContainerMacros.h>

#include <Messages/Messages.h>

#include "MonsterBuilder.h"

namespace CaterpillarMonster
{

namespace Animations
{
	static const float kKFDuration = 0.1f;

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
			Fragment('o', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen),
		};
		static const AsciiMesh kKF1(kKF1Fragments, 4, 1, IVec2(0, 0));

		static const Fragment kKF2Fragments[] = 
		{
			Fragment('O', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen),
		};
		static const AsciiMesh kKF2(kKF2Fragments, 4, 1, IVec2(0, 0));
		
		static const AsciiMesh kKeyFrames[] =
		{
			kKF1,
			kKF1,
			kKF1,
			kKF1,
			kKF1,
			kKF2,
			kKF1,
			kKF2,
		};

		static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFDuration, Animation::EPlaybackStyle_Loop);
	}

	namespace IdleFaceRight
	{
		static const Fragment kKF1Fragments[] = 
		{
			Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('o', ETextGreen),
		};
		static const AsciiMesh kKF1(kKF1Fragments, 4, 1, IVec2(-3, 0));

		static const Fragment kKF2Fragments[] = 
		{
			Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('-', ETextGreen), Fragment('O', ETextGreen),
		};
		static const AsciiMesh kKF2(kKF2Fragments, 4, 1, IVec2(-3, 0));
		
		static const AsciiMesh kKeyFrames[] =
		{
			kKF1,
			kKF1,
			kKF1,
			kKF1,
			kKF1,
			kKF2,
			kKF1,
			kKF2,
		};

		static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKFDuration, Animation::EPlaybackStyle_Loop);
	}
}

static const float	kIdleTime		= Animations::kKFDuration * gElemCount(Animations::IdleFaceRight::kKeyFrames);
static const int	kAttackStrength = 1;

class CaterpillarMonsterComponent
{
public:
	CaterpillarMonsterComponent() : mState(EIdle), mDirection(ELeft), mTimeUntilNextDecision(0.0f) { }
	~CaterpillarMonsterComponent() { }

	void Update(Entity inThis, float inFrameTime); 

private:
	enum EMovementDirection
	{
		ELeft,
		ERight,
	};

	enum EState
	{
		EIdle,
		EMoving,
	};

	EState				mState;
	EMovementDirection	mDirection;
	float				mTimeUntilNextDecision;

};

void CaterpillarMonsterComponent::Update(Entity inThis, float inFrameTime)
{
	if (inThis.GetComponent<MonsterComponent>()->IsDying())
	{
		return;
	}

	switch (mState)
	{
		case EIdle:
		{
			mTimeUntilNextDecision -= inFrameTime;

			if (mTimeUntilNextDecision > 0.0f)
			{
				// Early out.
				return;
			}
			else
			{
				// Reset timer, and continue to select new action.
				mTimeUntilNextDecision = kIdleTime;
			}
			break;
		}

		case EMoving:
		{
			if (!inThis.GetComponent<AnimationComponent>()->IsSelectedAnimationFinished())
			{
				// Early out.
				return;
			}
			else
			{
				// Update position, and attack all positions in between previous and last.
				auto posComp		= inThis.GetComponent<PositionComponent>();
				IVec2 position		= posComp->GetPosition();
				IVec2 stepDirection = (mDirection == ELeft) ? IVec2(-1, 0) : IVec2(1, 0);

				IVec2 positionStep1 = position + stepDirection;
				IVec2 newPosition	= positionStep1 + stepDirection;

				AttackMsg attackMsg1(inThis, positionStep1, stepDirection, AttackMsg::EEffect_PushBack, kAttackStrength);
				MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, positionStep1, attackMsg1);

				AttackMsg attackMsg2(inThis, newPosition, stepDirection, AttackMsg::EEffect_PushBack, kAttackStrength);
				MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, positionStep1, attackMsg2);

				posComp->SetPosition( newPosition );
			}
		}
	}
	
	// Select a new action
	enum EAction
	{
		EAction_Idle,
		EAction_TurnAround,
		EAction_Move,
	};

	EAction action		= EAction_Idle;
	EState	nextState	= (EState) (rand() % 2);

	switch (nextState)
	{
		case EIdle:
		{
			action = EAction_Idle;
			break;
		}

		case EMoving:
		{
			EMovementDirection newDirection	= (EMovementDirection) (rand() % 2);

			if (newDirection != mDirection)
			{
				action = EAction_TurnAround;
			}
			else
			{
				action = EAction_Move;
			}
			
			break;
		}
	}

	if (action != EAction_Idle)
	{
		// Validate whether or not we can move to new position.
		IVec2				position		= inThis.GetComponent<PositionComponent>()->GetPosition();

		EMovementDirection	newDirection	= (action == EAction_Move) ? mDirection : (EMovementDirection) (mDirection ^ 1);
		IVec2				step			= (newDirection == ELeft) ? IVec2(-1, 0) : IVec2(1, 0);

		
		bool isValidPos =	!CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, position + step)
						&&	!CollisionSystem::CollidesWithAnyEntity(*inThis.GetWorld(), inThis, position + step + step);

		if (!isValidPos)
		{
			action = EAction_Idle;
		}
	}
	
	// Update state and direction
	mState		= (action == EAction_Idle) ? EIdle : EMoving;
	mDirection	= (action != EAction_TurnAround) ? mDirection : (EMovementDirection) (mDirection ^ 1);

	// Select appropriate animation
	const Animation* nextAnimation = nullptr;

	switch (action)
	{
		case EAction_Idle:
		{
			switch (mDirection) // Use current direction
			{
				case ELeft:		nextAnimation = &Animations::IdleFaceLeft::kAnimation; break;
				case ERight:	nextAnimation = &Animations::IdleFaceRight::kAnimation; break;
			}
			break;
		}
		
		case EAction_TurnAround:
		{
			switch (mDirection)
			{
				case ELeft: nextAnimation = &Animations::MovementTurnLeft::kAnimation; break; 
				case ERight: nextAnimation = &Animations::MovementTurnRight::kAnimation; break;
			}
			break;
		}
		
		case EAction_Move:
		{
			switch (mDirection)
			{
				case ELeft: nextAnimation = &Animations::MovementLeft::kAnimation; break; 
				case ERight: nextAnimation = &Animations::MovementRight::kAnimation; break;
			}
			break;
		}
	}

	inThis.GetComponent<AnimationComponent>()->SetAnimation( *nextAnimation );

	// Attack in direction ahead of caterpillar.
	if (action != EAction_Idle)
	{
		IVec2 position		= inThis.GetComponent<PositionComponent>()->GetPosition();
		IVec2 stepDirection	= (mDirection == ELeft) ? IVec2(-1, 0) : IVec2(1, 0);
		IVec2 attackPos		= position + stepDirection;

		AttackMsg attackMsg(inThis, attackPos, stepDirection, AttackMsg::EEffect_PushBack, kAttackStrength);
		MessageHelpers::BroadcastMessageToEntitiesAtPosition(*inThis.GetWorld(), inThis, attackPos, attackMsg);
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
