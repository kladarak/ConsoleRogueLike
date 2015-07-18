#include "Animation.h"

Animation::Animation()
	: mCurrentKeyFrame	( 0	)
	, mKeyFrameDuration	( 0.0f )
	, mRunningTime		( 0.0f )
	, mPlaybackStyle	( EPlaybackStyle_Loop )
{
}

Animation::Animation(Animation&& inRHS)
	: mKeyFrames		( std::move(inRHS.mKeyFrames)	)
	, mCurrentKeyFrame	( inRHS.mCurrentKeyFrame		)
	, mKeyFrameDuration	( inRHS.mKeyFrameDuration		)
	, mRunningTime		( inRHS.mRunningTime			)
	, mPlaybackStyle	( inRHS.mPlaybackStyle			)
{
}

Animation::Animation(const Animation& inRHS)
	: mKeyFrames		( inRHS.mKeyFrames			)
	, mCurrentKeyFrame	( inRHS.mCurrentKeyFrame	)
	, mKeyFrameDuration	( inRHS.mKeyFrameDuration	)
	, mRunningTime		( inRHS.mRunningTime		)
	, mPlaybackStyle	( inRHS.mPlaybackStyle		)
{
}

Animation::Animation(const AsciiMesh* inMeshes, uint32_t inCount, float inKeyFrameDuration, EPlaybackStyle inPlaybackStyle)
	: mCurrentKeyFrame(0)
	, mKeyFrameDuration(inKeyFrameDuration)
	, mRunningTime(0.0f)
	, mPlaybackStyle(inPlaybackStyle)
{
	for (uint32_t i = 0; i < inCount; ++i)
	{
		mKeyFrames.push_back(inMeshes[i]);
	}
}

void Animation::Update(float inFrameTime)
{
	if (mKeyFrames.size() <= 1 || mKeyFrameDuration < 0.001f)
	{
		return;
	}

	mRunningTime += inFrameTime;
	
	while (mRunningTime > mKeyFrameDuration)
	{
		++mCurrentKeyFrame;

		if (mCurrentKeyFrame >= mKeyFrames.size())
		{
			switch (mPlaybackStyle)
			{
				case EPlaybackStyle_Once: mCurrentKeyFrame = mKeyFrames.size() - 1; break;
				case EPlaybackStyle_Loop: mCurrentKeyFrame = 0;						break;
			}
		}

		mRunningTime -= mKeyFrameDuration;
	}
}

const AsciiMesh& Animation::GetCurrentKeyFrame() const
{
	static const AsciiMesh kEmptyMesh;
	return mKeyFrames.size() > 0 ? mKeyFrames[mCurrentKeyFrame] : kEmptyMesh;
}

void Animation::Reset()
{
	mCurrentKeyFrame	= 0;
	mRunningTime		= 0.0f;
}
