#include "MapScreenState.h"

#include <algorithm>

#include <Maths/Maths.h>
#include <Input/InputBuffer.h>
#include <Renderer/RenderTargetWriter.h>
#include <Renderer/AsciiMesh.h>

#include <EntityComponent/Components/PositionComponent.h>

#include <Dungeon/DungeonMap.h>
#include <Dungeon/RoomEntity.h>

#include "GameData.h"
#include "ScreenConstants.h"

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
		gCastUCharToChar(197), // BottomLeft | BottomRight | TopRight | TopLeft
	};

	const char kVerticalWall	= gCastUCharToChar(179);
	const char kHorizontalWall	= gCastUCharToChar(196);

	const int	kViewMargin					= 5;
	const float kPlayerIconFlashDuration	= 0.5f;
}

MapScreenState::MapScreenState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
	, mBasicMap(0, 0)
	, mViewOffset(0, 0)
	, mPlayerIconFlashTime(0.0f)
{
	auto&	roomDataMap		= mGameData->mDungeonMap.GetRoomData();
	auto&	roomsVisited	= mGameData->mDungeonMap.GetRoomsVisited();
	int		colCount		= roomsVisited.GetColCount();
	int		rowCount		= roomsVisited.GetRowCount();

	RenderTargetWriter writer(colCount*3, rowCount*3);
	
	auto hasVisitedRoom = [&] (int inCol, int inRow)
	{
		bool inRange = gIsBetween(inCol, 0, colCount) && gIsBetween(inRow, 0, rowCount);
		return inRange && roomsVisited.Get(inCol, inRow);
	};

	auto ifWallWriteChar = [&] (int inCol, int inRow, EDoorSide inDoorSide, char inWallChar, int inX, int inY)
	{
		if (hasVisitedRoom(inCol, inRow) && !roomDataMap.Get(inCol, inRow).mDoors[inDoorSide])
		{
			writer.Write(inWallChar, inX, inY);
		}
	};

	for (int row = 0; row <= rowCount; ++row)
	{
		for (int col = 0; col <= colCount; ++col)
		{
			int mask = 0;
			mask |= hasVisitedRoom(col-1,	row-1)	? TopLeft		: 0;
			mask |= hasVisitedRoom(col,	row-1)		? TopRight		: 0;
			mask |= hasVisitedRoom(col-1,	row)	? BottomLeft	: 0;
			mask |= hasVisitedRoom(col,	row)		? BottomRight	: 0;

			char piece = kCornerPiece[mask];
			int x = col * 2;
			int y = row * 2;
			writer.Write(piece, x, y);

			ifWallWriteChar(col, row, EDoorSide_Left,	kVerticalWall,		x,		y+1);
			ifWallWriteChar(col, row, EDoorSide_Right,	kVerticalWall,		x+2,	y+1);
			ifWallWriteChar(col, row, EDoorSide_Top,	kHorizontalWall,	x+1,	y);
			ifWallWriteChar(col, row, EDoorSide_Bottom, kHorizontalWall,	x+1,	y+2);
		}
	}

	mBasicMap = writer.GetRenderTarget();
}

void MapScreenState::Update(float inFrameTime, const InputBuffer& inInput)
{
	if (inInput.IsPressed('m'))
	{
		RequestPopState();
	}

	if (inInput.IsPressed('a'))
	{
		mViewOffset.mX = std::min(mViewOffset.mX + 1, mBasicMap.GetColCount() + kViewMargin);
	}
	else if (inInput.IsPressed('d'))
	{
		mViewOffset.mX = std::max(mViewOffset.mX - 1, -kViewMargin);
	}
	
	if (inInput.IsPressed('w'))
	{
		mViewOffset.mY = std::min(mViewOffset.mY + 1, mBasicMap.GetRowCount() + kViewMargin);
	}
	else if (inInput.IsPressed('s'))
	{
		mViewOffset.mY = std::max(mViewOffset.mY - 1, -kViewMargin);
	}

	mPlayerIconFlashTime += inFrameTime;
}

std::string MapScreenState::GetRenderBuffer() const
{
	RenderTargetWriter writer(ScreenConstants::EViewPortWidth, ScreenConstants::EViewPortHeight);

	writer.Write(mBasicMap, mViewOffset.mX, mViewOffset.mY);

	if (fmodf(mPlayerIconFlashTime / kPlayerIconFlashDuration, 2.0f) < 1.0f)
	{
		auto playerPos = mGameData->mPlayer.GetComponent<PositionComponent>()->GetPosition();
		
		int col = playerPos.mX / ERoomDimensions_Width;
		int row = playerPos.mY / ERoomDimensions_Height;
		int x = (col * 2) + 1;
		int y = (row * 2) + 1;
		x += mViewOffset.mX;
		y += mViewOffset.mY;

		writer.Write('O', x, y);
	}

	return writer.GetRenderBuffer();
}
