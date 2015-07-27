#include "InGameState.h"

#include <GameEntities/Player/PlayerEntity.h>

#include <Dungeon/DungeonFactory.h>
#include <Dungeon/RoomEntity.h>
#include <Dungeon/LevelData.h>

#include <EntityComponent/Systems/AnimationSystem.h>
#include <EntityComponent/Systems/InputHandlerSystem.h>
#include <EntityComponent/Systems/PositionSystem.h>
#include <EntityComponent/Systems/ProgramSystem.h>
#include <EntityComponent/Systems/RenderSystem.h>
#include <EntityComponent/Systems/TriggerSystem.h>

#include <Input/InputBuffer.h>

#include <GameSystems/ItemDropHandler.h>

#include <Renderer/RenderTarget.h>


#include <Inventory/Items/DoorKey.h>

#include <Messages/Messages.h>

#include "ScreenConstants.h"
#include "GameData.h"

InGameState::InGameState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
{
	mGameData->mDungeonMap = DungeonFactory::Generate(mWorld, mMessageBroadcaster, inGameData, gGenerateLevelData(inGameData->mCurrentLevel));

	mCameraSystem.Init(mWorld, mGameData->mDungeonMap);

	mGameData->mPlayer = PlayerEntity::Create(mWorld, mMessageBroadcaster, inGameData);

	mHUD.Init(mMessageBroadcaster, inGameData);

	mMessageBroadcaster.Register<BackToStartMenuMsg>( [&] (const BackToStartMenuMsg&)
	{
		mGameData->mCurrentLevel = 1;
		mGameData->mPlayerData.mMaxHealth = 3;
		mGameData->mPlayerData.mInventory.DeleteAll();
		mGameData->mPlayerData.SetItemInSlot(nullptr, Player::EItemSlot_Slot0);
		mGameData->mPlayerData.SetItemInSlot(nullptr, Player::EItemSlot_Slot1);
		RequestGoToState(EGameState_StartMenu);
	} );

	mMessageBroadcaster.Register<PlayerWentDownStairs>( [&] (const PlayerWentDownStairs&) 
	{
		mGameData->mCurrentLevel++;
		RequestGoToState(EGameState_StartLevelIntro);
	} );

	ItemDropHandler::Register(mWorld, mMessageBroadcaster, mGameData);
}

InGameState::~InGameState()
{
	// Remove all Keys.
	{
		auto& playerData = mGameData->mPlayerData;
		auto& inventory = playerData.mInventory;
		
		std::vector<ItemBase*> allItems = inventory.GetAllItems();

		for (auto item : allItems)
		{
			if (item->GetName() == DoorKey::kName)
			{
				if (item == playerData.GetItemInSlot(Player::EItemSlot_Slot0))
				{
					playerData.SetItemInSlot(nullptr, Player::EItemSlot_Slot0);
				}

				if (item == playerData.GetItemInSlot(Player::EItemSlot_Slot1))
				{
					playerData.SetItemInSlot(nullptr, Player::EItemSlot_Slot1);
				}

				inventory.RemoveAndDeleteItem(item);
			}
		}
	}

	// Clear Player.
	mGameData->mPlayer = Entity();
}

void InGameState::Update(float inFrameTime, const InputBuffer& inInput)
{
	if (inInput.IsPressed('\t'))
	{
		RequestPushState(EGameState_InventoryView);
	}
	
	if (inInput.IsPressed('m'))
	{
		RequestPushState(EGameState_MapView);
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

RenderTarget InGameState::GetRenderTarget() const
{
	using namespace ScreenConstants;
	
	const int gameWidth		= ERoomDimensions_Width;
	const int gameHeight	= ERoomDimensions_Height;
	const int screenWidth	= gameWidth;
	const int screenHeight	= mHUD.GetTopBarHeight() + gameHeight + mHUD.GetBottomBarHeight();

	int y = 0;

	RenderTarget mainRenderTarget(screenWidth, screenHeight);

	mainRenderTarget.Write( mHUD.GetTopBarRenderTarget(), 0, y );
	y += mHUD.GetTopBarHeight();
	
	{
		RenderTarget gameRenderTarget(gameWidth, gameHeight);
		IVec2 cameraPosition = mCameraSystem.GetCameraPosition();
		RenderSystem::Render( const_cast<InGameState*>(this)->mWorld, cameraPosition, gameRenderTarget ); // Ooh, nasty const_cast... Must improve World's interface...

		mainRenderTarget.Write( gameRenderTarget, 0, y );
		y += gameHeight;
	}

	mainRenderTarget.Write( mHUD.GetBottomBarRenderTarget(), 0, y );

	int halfHeight = screenHeight / 2;
	mainRenderTarget.Write( mHUD.GetOverlayRenderTarget(), 10, halfHeight );

	return mainRenderTarget;
}
