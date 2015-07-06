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

#include <Messages/Messages.h>

#include "ScreenConstants.h"
#include "GameData.h"

InGameState::InGameState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
{
	mDungeonMap = DungeonFactory::Generate(mWorld, mMessageBroadcaster);
	
	mCameraSystem.Init(mWorld, mDungeonMap);

	mPlayer = PlayerEntity::Create(mWorld, mMessageBroadcaster, mDungeonMap, inGameData);

	mHUD.Init(mMessageBroadcaster, &inGameData->mPlayerData, mPlayer);

	mMessageBroadcaster.Register<BackToStartMenuMsg>( [&] (const BackToStartMenuMsg&) { RequestGoToState(EGameState_StartMenu); } );
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
