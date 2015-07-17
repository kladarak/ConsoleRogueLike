#include "Game.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Windows.h"

#include <StateMachine/StateBase.h>

#include <Inventory/Items/Sword.h>
#include <Inventory/Items/Bow.h>
#include <Inventory/Items/Shield.h>
#include <Inventory/Items/Bomb.h>

Game::Game()
	: mStateMachineMessageHandler(mStateMachine, mGameData)
	, mIsRunning(false)
{
	auto sword	= new Sword();
	auto bow	= new Bow();
	mGameData.mPlayerData.mInventory.AddItem( sword );
	mGameData.mPlayerData.mInventory.AddItem( bow );
	mGameData.mPlayerData.mInventory.AddItem( new Shield() );
	mGameData.mPlayerData.mInventory.AddItem( new Bomb() );

	mGameData.mPlayerData.SetItemInSlot( sword,	Player::EItemSlot_Slot0 );
	mGameData.mPlayerData.SetItemInSlot( bow,	Player::EItemSlot_Slot1 );
}

int Game::Run()
{
	Init();

	while (mIsRunning)
	{
		Update();
		Render();
	}

	return 0;
}

void Game::Init()
{
	srand( (uint32_t) time(NULL) );

	mTimer.Start();
	mInputMonitor.StartMonitoring();

	mIsRunning = true;
}

void Game::Update()
{
	if (mStateMachineMessageHandler.HasStateChangeRequests())
	{
		mStateMachineMessageHandler.ProcessStateChangeRequests();
		system("cls");
	}

	mTimer.Tick();

	float	frameTime	= mTimer.GetDeltaTime();
	auto	inputBuffer = mInputMonitor.ConsumeBuffer();

	mStateMachine.GetTop()->Update(frameTime, inputBuffer);
}

void Game::Render()
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	
	{
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(output, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(output, &cursorInfo);
	}

	RenderTarget renderTarget = mStateMachine.GetTop()->GetRenderTarget();

	renderTarget.ForEach( [&] (size_t inCol, size_t inRow, const Fragment& inFragment)
	{
		COORD pos = { (short) inCol, (short) inRow };
		SetConsoleCursorPosition( output, pos );
		SetConsoleTextAttribute( output, (WORD) inFragment.mColour );
		printf( "%c", inFragment.mChar );
	} );

	printf("\n\n%04i FPS (%03.3fms)", (uint32_t) (1.0f / mTimer.GetDeltaTime()), mTimer.GetDeltaTime() * 1000.0f);
}
