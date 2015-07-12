#include "MapScreenState.h"

#include <Input/InputBuffer.h>
#include <Renderer/RenderTargetWriter.h>
#include <Renderer/AsciiMesh.h>

#include <Dungeon/DungeonMap.h>

#include "GameData.h"

namespace
{
	enum ECorner
	{
		TopLeft		= 1 << 0,
		TopRight	= 1 << 1,
		BottomLeft	= 1 << 2,
		BottomRight = 1 << 3
	};

	const char kCornerPiece[]
	{
		' ',
		gCastUCharToChar(217), // TopLeft
		gCastUCharToChar(192), // TopRight
		gCastUCharToChar(193), // TopLeft | TopRight

		gCastUCharToChar(191), // BottomLeft
		gCastUCharToChar(180), // BottomLeft | TopLeft
		gCastUCharToChar(197), // BottomLeft | TopRight
		gCastUCharToChar(197), // BottomLeft | TopLeft | TopRight
		
		gCastUCharToChar(218), // BottomRight
		gCastUCharToChar(197), // BottomRight | TopLeft
		gCastUCharToChar(195), // BottomRight | TopRight
		gCastUCharToChar(197), // BottomRight | TopLeft | TopRight
		
		gCastUCharToChar(194), // BottomLeft | BottomRight
		gCastUCharToChar(197), // BottomLeft | BottomRight | TopLeft
		gCastUCharToChar(197), // BottomLeft | BottomRight | TopRight
		gCastUCharToChar(197), // BottomLeft | BottomRight | TopRight \ TopLeft
	};

	//const char kRoomPixels[] = 
	//{
	//	gCastUCharToChar(218),	'-',	gCastUCharToChar(191),
	//	'|',					' ',	'|',
	//	gCastUCharToChar(192),	'-',	gCastUCharToChar(217),
	//};
	
	const char kRoomPixels[] = 
	{
		gCastUCharToChar(218), gCastUCharToChar(194), gCastUCharToChar(191),
		gCastUCharToChar(195), gCastUCharToChar(197), gCastUCharToChar(180),
		gCastUCharToChar(192), gCastUCharToChar(193), gCastUCharToChar(217),
	};

	const AsciiMesh kRoomMesh(kRoomPixels, 3, 3);
}

MapScreenState::MapScreenState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
{
}

void MapScreenState::Update(float /*inFrameTime*/, const InputBuffer& inInput)
{
	if (inInput.IsPressed('p'))
	{
		RequestPopState();
	}
}

std::string MapScreenState::GetRenderBuffer() const
{
	auto&	dungeonMap	= mGameData->mDungeonMap;
	size_t	colCount	= dungeonMap.GetColCount();
	size_t	rowCount	= dungeonMap.GetRowCount();

	auto isValidRoom = [&] (size_t inCol, size_t inRow)
	{
		bool inRange = inCol < colCount && inRow < rowCount;
		return inRange && dungeonMap.GetRoomData().Get(inCol, inRow).mIsValid;
	};

	RenderTargetWriter renderTarget(dungeonMap.GetColCount()*3, dungeonMap.GetRowCount()*3);
	
	for (size_t row = 0; row < dungeonMap.GetRowCount(); ++row)
	{
		for (size_t col = 0; col < dungeonMap.GetColCount(); ++col)
		{
			int mask = 0;
			mask |= isValidRoom(col,	row)	? TopLeft		: 0;
			mask |= isValidRoom(col+1,	row)	? TopRight		: 0;
			mask |= isValidRoom(col,	row+1)	? BottomLeft	: 0;
			mask |= isValidRoom(col+1,	row+1)	? BottomRight	: 0;

			char piece = kCornerPiece[mask];
			int x = col * 2;
			int y = row * 2;
			renderTarget.Write(piece, x, y);
		}
	}

	return renderTarget.GetRenderBuffer();
}
