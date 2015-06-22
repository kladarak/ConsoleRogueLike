#include "InGamePlayScreen.h"

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

void InGamePlayScreen::Init()
{
	mRunning = true;

	mDungeonMap = DungeonFactory::Generate(mWorld, mMessageBroadcaster);
	
	mCameraSystem.Init(mWorld, mDungeonMap);

	mPlayer = PlayerEntity::Create(mWorld, mMessageBroadcaster);

	mHUD.Init(mMessageBroadcaster, mPlayer);

	mMessageBroadcaster.Register<BackToStartMenuMsg>( [&] (const BackToStartMenuMsg&) { mRunning = false; } );
}

bool InGamePlayScreen::Update(float inFrameTime, const InputBuffer& inInput)
{
	TriggerSystem::HandleDestroyedEntities(mWorld);
	mWorld.FlushDestroyedEntities();

	PositionSystem::SwapPositionBuffers(mWorld);

	InputHandlerSystem::Update(mWorld, inInput);

	ProgramSystem::Update(mWorld, inFrameTime);
	
	TriggerSystem::Update(mWorld);

	AnimationSystem::Update(mWorld, inFrameTime);

	mHUD.Update(inFrameTime);

	return mRunning;
}

std::string InGamePlayScreen::GetRenderBuffer()
{
	using namespace ScreenConstants;
	
	const int width		= EMapCols+1;
	const int height	= mHUD.GetTopBarHeight() + EMapRows + mHUD.GetBottomBarHeight();

	int y = 0;

	RenderTargetWriter renderTargetWriter(width, height);

	renderTargetWriter.Write( mHUD.GetTopBarRenderBuffer(), 0, y );
	y += mHUD.GetTopBarHeight();
	
	RenderTarget renderTarget(EMapCols+1, EMapRows);
	IVec2 cameraPosition = mCameraSystem.GetCameraPosition();
	RenderSystem::Render(mWorld, cameraPosition, renderTarget);

	renderTargetWriter.Write( renderTarget.GetBuffer(), 0, y );
	y += EMapRows;
	
	renderTargetWriter.Write( mHUD.GetBottomBarRenderBuffer(), 0, y );

	int halfHeight = height / 2;
	renderTargetWriter.Write( mHUD.GetOverlayBuffer(), 10, halfHeight );

	return renderTargetWriter.GetRenderBuffer();
}
