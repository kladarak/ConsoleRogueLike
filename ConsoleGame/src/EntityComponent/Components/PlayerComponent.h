#pragma once

#include <vector>

#include <EntityComponentSystem/World/World.h>

#include <Maths/IVec2.h>
#include <GameEntities/Player/PlayerEnums.h>

class AttackMsg;
class InputBuffer;
class ItemBase;
class PlayerBehaviourBase;
class PlayerData;
class MessageBroadcaster;

class PlayerComponent
{
public:
	PlayerComponent(PlayerData* inPlayerData);
	PlayerComponent(PlayerComponent&& inRHS);

	~PlayerComponent();
	
	void						HandleInput(const InputBuffer& inBuffer);
	void						Update(Entity inPlayer, float inFrameTime);
	void						OnAttacked(Entity inPlayer, const AttackMsg& inAttackMsg, MessageBroadcaster& inBroadcaster);

private:
	void						UpdateState(Entity inPlayer);
	void						UpdateOrientation(Entity inPlayer);
	void						UpdatePosition(Entity inPlayer);
	void						UpdateBehaviour(Entity inPlayer, float inFrameTime);
	void						UpdateAnimation(Entity inPlayer, float inFrameTime);
	
	enum EState
	{
		EState_Idle,
		EState_UseItem,
		EState_Dead
	};
	
	struct Intention
	{
		IVec2				mMovement;
		EOrientation		mFacingDirection;
		EState				mState;
		Player::EItemSlot	mUseItemSlot;

		Intention() 
			: mMovement			(0, 0)
			, mFacingDirection	(EOrientation_Count)
			, mState			(EState_Idle)
			, mUseItemSlot		(Player::EItemSlot_None)
		{
		}
	};

	PlayerData*					mPlayerData;

	Intention					mIntention;

	EState						mState;
	
	PlayerBehaviourBase*		mIdleBehaviour;
	PlayerBehaviourBase*		mDeadBehaviour;
	PlayerBehaviourBase*		mCurrentBehaviour;

	IVec2						mLastSafePosition;
	float						mDamagedFlashTimeRemaining;
	
};
