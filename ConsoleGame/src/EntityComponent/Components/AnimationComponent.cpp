#include "AnimationComponent.h"

#include <Core/Debug/Assert.h>

AnimationComponent::AnimationComponent(AnimationComponent&& inRHS)
	: mAnimations		( std::move(inRHS.mAnimations)	)
	, mSelectedAnimation( inRHS.mSelectedAnimation		)
{
}

AnimationComponent::AnimationComponent(const AnimationComponent& inRHS)
	: mAnimations		( inRHS.mAnimations			)
	, mSelectedAnimation( inRHS.mSelectedAnimation	)
{
}

AnimationComponent::AnimationComponent(const Animation& inAnimation)
	: mSelectedAnimation(0)
{
	mAnimations.push_back( inAnimation );
}

AnimationComponent::AnimationComponent(const Animation* inAnimations, uint32_t inCount)
	: mSelectedAnimation(0)
{
	for (int i = 0; i < inCount; ++i)
	{
		mAnimations.push_back( inAnimations[i] );
	}
}

void AnimationComponent::SetSelectedAnimation(uint32_t inSelected)
{
	assert(inSelected < mAnimations.size());
	mSelectedAnimation = inSelected;
}

void AnimationComponent::Update(float inFrameTime)
{
	if (mAnimations.size() > 0)
	{
		assert(mSelectedAnimation < mAnimations.size());
		mAnimations[mSelectedAnimation].Update(inFrameTime);
	}
}

const AsciiMesh& AnimationComponent::GetCurrentKeyFrame() const
{
	static const AsciiMesh kEmptyMesh;
	assert(mSelectedAnimation < mAnimations.size() || mAnimations.size() == 0);
	return mAnimations.size() > 0 ? mAnimations[mSelectedAnimation].GetCurrentKeyFrame() : kEmptyMesh;
}
