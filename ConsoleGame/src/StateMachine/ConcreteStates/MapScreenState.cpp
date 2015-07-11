#include "MapScreenState.h"

#include <Input/InputBuffer.h>
#include <Renderer/RenderTargetWriter.h>

#include <Dungeon/DungeonMap.h>

#include "GameData.h"

MapScreenState::MapScreenState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
{
}

void MapScreenState::Update(float inFrameTime, const InputBuffer& inInput)
{
	if (inInput.IsPressed('p'))
	{
		RequestPopState();
	}
}

std::string MapScreenState::GetRenderBuffer() const
{
	auto& dungeonMap = mGameData->mDungeonMap;

	const int kMargin = 2;
	RenderTargetWriter renderTarget(dungeonMap.GetColCount() + kMargin*2, dungeonMap.GetRowCount() + kMargin*2);

	dungeonMap.ForEach( [&] (size_t inX, size_t inY, const Entity& inRoom)
	{
		if (inRoom.IsValid())
		{
			renderTarget.Write('X', kMargin + inX, kMargin + inY);
		}
	} );

	return renderTarget.GetRenderBuffer();
}
