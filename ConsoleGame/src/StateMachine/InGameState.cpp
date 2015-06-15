#include "InGameState.h"

#include <GameEntities/RoomEntity.h>
#include <GameEntities/Player/PlayerEntity.h>
#include <GameEntities/SpinnerEntity.h>
#include <Dungeon/DungeonFactory.h>

#include <EntityComponent/Systems/AnimationSystem.h>
#include <EntityComponent/Systems/InputHandlerSystem.h>
#include <EntityComponent/Systems/PositionSystem.h>
#include <EntityComponent/Systems/ProgramSystem.h>
#include <EntityComponent/Systems/RenderSystem.h>
#include <EntityComponent/Systems/TriggerSystem.h>

#include <Renderer/RenderTarget.h>

#include "ScreenConstants.h"

void InGameState::Reset()
{
	*this = InGameState();
}

void InGameState::Init()
{
	auto rooms = DungeonFactory::Generate(mWorld, mMessageBroadcaster);
	
	mDungeonMap.Init(rooms);
	
	mCameraSystem.Init(mWorld, mDungeonMap);

	auto player = PlayerEntity::Create(mWorld);

	mHUD.Init(mMessageBroadcaster, player);
}

EGameState InGameState::Update(float inFrameTime, const InputBuffer& inInput)
{
	mWorld.FlushDestroyedEntities();

	PositionSystem::SwapPositionBuffers(mWorld);

	InputHandlerSystem::Update(mWorld, inInput);

	ProgramSystem::Update(mWorld, inFrameTime);

	TriggerSystem::Update(mWorld);

	AnimationSystem::Update(mWorld, inFrameTime);

	return EGameState_InGame;
}

std::string InGameState::GetRenderBuffer()
{
	using namespace ScreenConstants;

	std::string outBuffer;

	outBuffer += mHUD.GetTopBarRenderBuffer();
	
	RenderTarget renderTarget(EMapCols+1, EMapRows);
	IVec2 cameraPosition = mCameraSystem.GetCameraPosition();
	RenderSystem::Render(mWorld, cameraPosition, renderTarget);

	outBuffer += renderTarget.GetBuffer();

	outBuffer += mHUD.GetBottomBarRenderBuffer();

	return outBuffer;
}
