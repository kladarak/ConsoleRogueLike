#include "MapScreenState.h"

#include <algorithm>

#include <Maths/Maths.h>
#include <Input/InputBuffer.h>

#include <Renderer/AsciiMesh.h>

#include <EntityComponent/Components/PositionComponent.h>

#include <GameEntities/Player/PlayerConstants.h>
#include <Dungeon/DungeonMap.h>
#include <Dungeon/RoomEntity.h>

#include <UI/Elements/BorderedFrame.h>

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

	const Fragment kCornerPiece[]
	{
		' ',
		Fragment( gCastUCharToChar(217), ETextDarkYellow), // TopLeft
		Fragment( gCastUCharToChar(192), ETextDarkYellow), // TopRight
		Fragment( gCastUCharToChar(193), ETextDarkYellow), // TopLeft | TopRight

		Fragment( gCastUCharToChar(191), ETextDarkYellow), // BottomLeft
		Fragment( gCastUCharToChar(180), ETextDarkYellow), // BottomLeft | TopLeft
		Fragment( gCastUCharToChar(197), ETextDarkYellow), // BottomLeft | TopRight
		Fragment( gCastUCharToChar(197), ETextDarkYellow), // BottomLeft | TopLeft | TopRight

		Fragment( gCastUCharToChar(218), ETextDarkYellow), // BottomRight
		Fragment( gCastUCharToChar(197), ETextDarkYellow), // BottomRight | TopLeft
		Fragment( gCastUCharToChar(195), ETextDarkYellow), // BottomRight | TopRight
		Fragment( gCastUCharToChar(197), ETextDarkYellow), // BottomRight | TopLeft | TopRight

		Fragment( gCastUCharToChar(194), ETextDarkYellow), // BottomLeft | BottomRight
		Fragment( gCastUCharToChar(197), ETextDarkYellow), // BottomLeft | BottomRight | TopLeft
		Fragment( gCastUCharToChar(197), ETextDarkYellow), // BottomLeft | BottomRight | TopRight
		Fragment( gCastUCharToChar(197), ETextDarkYellow), // BottomLeft | BottomRight | TopRight | TopLeft
	};

	const Fragment kVerticalWall	= Fragment( gCastUCharToChar(179), ETextDarkYellow) ;
	const Fragment kHorizontalWall	= Fragment( gCastUCharToChar(196), ETextDarkYellow) ;

	const int	kViewMargin					= 5;
	const float kPlayerIconFlashDuration	= 0.5f;
	
	static const int kWindowWidth	= ScreenConstants::EViewPortWidth;
	static const int kWindowHeight	= ScreenConstants::EViewPortHeight;
	static const int kWindowMargin	= 2;
	static const int kMapViewWidth	= kWindowWidth - (kWindowMargin * 2);
	static const int kMapViewHeight = kWindowHeight - (kWindowMargin * 2);

	IVec2 sCalculatePlayerIconPosition(const Entity& inPlayer)
	{
		auto playerPos = inPlayer.GetComponent<PositionComponent>()->GetPosition();
		
		int col = playerPos.mX / kMapViewWidth;
		int row = playerPos.mY / kMapViewHeight;
		int x = (col * 2) + 1;
		int y = (row * 2) + 1;

		return IVec2(x, y);
	}
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

	RenderTarget renderTarget(colCount*3, rowCount*3);
	
	auto hasVisitedRoom = [&] (int inCol, int inRow)
	{
		bool inRange = gIsBetween(inCol, 0, colCount) && gIsBetween(inRow, 0, rowCount);
		return inRange && roomsVisited.Get(inCol, inRow);
	};

	auto ifWallWriteChar = [&] (int inCol, int inRow, EDoorSide inDoorSide, const Fragment& inWallChar, int inX, int inY)
	{
		if (hasVisitedRoom(inCol, inRow) && !roomDataMap.Get(inCol, inRow).mDoors[inDoorSide])
		{
			renderTarget.Write(inWallChar, inX, inY);
		}
	};

	for (int row = 0; row <= rowCount; ++row)
	{
		for (int col = 0; col <= colCount; ++col)
		{
			int mask = 0;
			mask |= hasVisitedRoom(col-1,	row-1)	? TopLeft		: 0;
			mask |= hasVisitedRoom(col,		row-1)	? TopRight		: 0;
			mask |= hasVisitedRoom(col-1,	row)	? BottomLeft	: 0;
			mask |= hasVisitedRoom(col,		row)	? BottomRight	: 0;

			auto& piece = kCornerPiece[mask];
			int x = col * 2;
			int y = row * 2;
			renderTarget.Write(piece, x, y);

			ifWallWriteChar(col, row, EDoorSide_Left,	kVerticalWall,		x,		y+1);
			ifWallWriteChar(col, row, EDoorSide_Right,	kVerticalWall,		x+2,	y+1);
			ifWallWriteChar(col, row, EDoorSide_Top,	kHorizontalWall,	x+1,	y);
			ifWallWriteChar(col, row, EDoorSide_Bottom, kHorizontalWall,	x+1,	y+2);
		}
	}

	mBasicMap = renderTarget;

	IVec2 playerIconPos = sCalculatePlayerIconPosition(mGameData->mPlayer);
	IVec2 mapCentre(kMapViewWidth/2, kMapViewHeight/2);
	mViewOffset = mapCentre - playerIconPos;
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

RenderTarget MapScreenState::GetRenderTarget() const
{
	RenderTarget mapView(kMapViewWidth, kMapViewHeight);

	mapView.Write(mBasicMap, mViewOffset.mX, mViewOffset.mY);

	if (fmodf(mPlayerIconFlashTime / kPlayerIconFlashDuration, 2.0f) < 1.0f)
	{
		IVec2 playerIconPos = sCalculatePlayerIconPosition(mGameData->mPlayer);
		playerIconPos += mViewOffset;
		mapView.Write(PlayerConstants::kSprite, playerIconPos.mX, playerIconPos.mY);
	}
	
	RenderTarget window = BorderedFrame(kWindowWidth, kWindowHeight).Render();
	window.Write(" Map ", 3, 0);
	window.Write(mapView, kWindowMargin, kWindowMargin);

	return window;
}
