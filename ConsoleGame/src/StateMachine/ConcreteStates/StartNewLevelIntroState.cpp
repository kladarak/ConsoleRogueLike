#include "StartNewLevelIntroState.h"

#include <algorithm>
#include <Maths/Maths.h>


#include <Messages/Messages.h>

#include "GameData.h"

namespace
{
	const float	kAnimationDuration	= 0.5f;
	const float	kStateDuration		= 1.0f;
}

StartNewLevelIntroState::StartNewLevelIntroState(MessageBroadcaster* inStateMachineMsgBroadcaster, GameData* inGameData)
	: StateBase(inStateMachineMsgBroadcaster, inGameData)
	, mAnimTime(0.0f)
{
}

void StartNewLevelIntroState::Update(float inFrameTime, const InputBuffer& /*inInput*/)
{
	mAnimTime += inFrameTime;

	if (mAnimTime > kStateDuration)
	{
		RequestGoToState( GoToStateMsg(EGameState_InGame) );
	}
}

RenderTarget StartNewLevelIntroState::GetRenderTarget() const
{
	std::string levelMsg	= "Level " + std::to_string( mGameData->mCurrentLevel );

	float	t				= gFraction(mAnimTime, 0.0f, kAnimationDuration);
	size_t	lettersToShow	= (size_t) gLerp2D(t, 0.0f, (float) levelMsg.size());

	lettersToShow			= std::min(lettersToShow, levelMsg.size());

	levelMsg = levelMsg.substr(0, lettersToShow);

	RenderTarget renderTarget(20, 10);
	
	renderTarget.Write( levelMsg, ETextWhite, 10, 5 );

	return renderTarget;
}
