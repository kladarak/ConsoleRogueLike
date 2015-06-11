#include "AnimationComponent.h"


AnimationComponent::AnimationComponent()
	: mCurrentKeyFrame(0)
	, mKeyFrameDuration(0.0f)
	, mRunningTime(0.0f)
{
}

AnimationComponent::AnimationComponent(AnimationComponent&& inRHS)
	: mKeyFrames		( std::move(inRHS.mKeyFrames)	)
	, mCurrentKeyFrame	( inRHS.mCurrentKeyFrame		)
	, mKeyFrameDuration	( inRHS.mKeyFrameDuration		)
	, mRunningTime		( inRHS.mRunningTime			)
{
}

AnimationComponent::AnimationComponent(const AnimationComponent& inRHS)
	: mKeyFrames		( inRHS.mKeyFrames			)
	, mCurrentKeyFrame	( inRHS.mCurrentKeyFrame	)
	, mKeyFrameDuration	( inRHS.mKeyFrameDuration	)
	, mRunningTime		( inRHS.mRunningTime		)
{
}

AnimationComponent::AnimationComponent(const AsciiMesh* inMeshes, int inCount, float inKeyFrameDuration)
	: mCurrentKeyFrame(0)
	, mKeyFrameDuration(inKeyFrameDuration)
	, mRunningTime(0.0f)
{
	for (int i = 0; i < inCount; ++i)
	{
		mKeyFrames.push_back(inMeshes[i]);
	}
}

void AnimationComponent::Update(float inFrameTime)
{
	if (mKeyFrames.size() <= 1 || mKeyFrameDuration < 0.001f)
	{
		return;
	}

	mRunningTime += inFrameTime;
	
	while (mRunningTime > mKeyFrameDuration)
	{
		mCurrentKeyFrame = (mCurrentKeyFrame + 1) % mKeyFrames.size();
		mRunningTime -= mKeyFrameDuration;
	}
}

const AsciiMesh& AnimationComponent::GetCurrentKeyFrame() const
{
	static const AsciiMesh kEmptyMesh;
	return mKeyFrames.size() > 0 ? mKeyFrames[mCurrentKeyFrame] : kEmptyMesh;
}
