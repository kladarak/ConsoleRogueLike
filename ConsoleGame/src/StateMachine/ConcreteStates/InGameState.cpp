#include "InGameState.h"

#include <GameEntities/Player/PlayerEntity.h>
#include <GameEntities/SpinnerEntity.h>
#include <Dungeon/DungeonFactory.h>

#include <EntityComponent/Systems/AnimationSystem.h>
#include <EntityComponent/Systems/InputHandlerSystem.h>
#include <EntityComponent/Systems/PositionSystem.h>
#include <EntityComponent/Systems/ProgramSystem.h>
#include <EntityComponent/Systems/RenderSystem.h>
#include <EntityComponent/Systems/TriggerSystem.h>

#include <Input/InputBuffer.h>

#include <Renderer/RenderTarget.h>
#include <Renderer/RenderTargetWriter.h>

#include <Inventory/Items/DoorKey.h>

#include <Messages/Messages.h>

#include "ScreenConstants.h"
#include "GameData.h"

InGameState::InGameState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
{
	// If this is a new level, remove DoorKey from inventory.
	// TODO: Improve cleanup of states/levels.
	{
		auto& playerData = mGameData->mPlayerData;
		auto& inventory = playerData.mInventory;
		
		auto key = inventory.FindItem(DoorKey::kName);

		if (key == playerData.GetItemInSlot(Player::EItemSlot_Slot0))
		{
			playerData.SetItemInSlot(nullptr, Player::EItemSlot_Slot0);
		}

		if (key == playerData.GetItemInSlot(Player::EItemSlot_Slot1))
		{
			playerData.SetItemInSlot(nullptr, Player::EItemSlot_Slot1);
		}

		inventory.RemoveAndDeleteItem(key);
	}

	mDungeonMap = DungeonFactory::Generate(mWorld, mMessageBroadcaster, inGameData);
	
	mCameraSystem.Init(mWorld, mDungeonMap);

	mPlayer = PlayerEntity::Create(mWorld, mMessageBroadcaster, mDungeonMap, inGameData);

	mHUD.Init(mMessageBroadcaster, &inGameData->mPlayerData, mPlayer);

	mMessageBroadcaster.Register<BackToStartMenuMsg>( [&] (const BackToStartMenuMsg&)
	{
		mGameData->mCurrentLevel = 1;
		RequestGoToState(EGameState_StartMenu);
	} );

	mMessageBroadcaster.Register<PlayerWentDownStairs>( [&] (const PlayerWentDownStairs&) 
	{
		mGameData->mCurrentLevel++;
		RequestGoToState(EGameState_StartLevelIntro);
	} );
}

void InGameState::Update(float inFrameTime, const InputBuffer& inInput)
{
	if (inInput.IsPressed('\t'))
	{
		RequestPushState(EGameState_InventoryView);
	}

	TriggerSystem::HandleDestroyedEntities(mWorld);
	mWorld.FlushDestroyedEntities();

	PositionSystem::SwapPositionBuffers(mWorld);

	InputHandlerSystem::Update(mWorld, inInput);

	ProgramSystem::Update(mWorld, inFrameTime);
	
	TriggerSystem::Update(mWorld);

	AnimationSystem::Update(mWorld, inFrameTime);

	mHUD.Update(inFrameTime);
}

std::string InGameState::GetRenderBuffer() const
{
	using namespace ScreenConstants;
	
	const int gameWidth		= (EMapCols+1);
	const int gameHeight	= EMapRows;
	const int screenWidth	= gameWidth;
	const int screenHeight	= mHUD.GetTopBarHeight() + gameHeight + mHUD.GetBottomBarHeight();

	int y = 0;

	RenderTargetWriter renderTargetWriter(screenWidth, screenHeight);

	renderTargetWriter.Write( mHUD.GetTopBarRenderBuffer(), 0, y );
	y += mHUD.GetTopBarHeight();
	
	RenderTarget renderTarget(gameWidth, gameHeight);
	IVec2 cameraPosition = mCameraSystem.GetCameraPosition();
	RenderSystem::Render( const_cast<InGameState*>(this)->mWorld, cameraPosition, renderTarget ); // Ooh, nasty const_cast... Must improve World's interface...

	renderTargetWriter.Write( renderTarget.GetBuffer(), 0, y );
	y += gameHeight;
	
	renderTargetWriter.Write( mHUD.GetBottomBarRenderBuffer(), 0, y );

	int halfHeight = screenHeight / 2;
	renderTargetWriter.Write( mHUD.GetOverlayBuffer(), 10, halfHeight );

	return renderTargetWriter.GetRenderBuffer();
}
