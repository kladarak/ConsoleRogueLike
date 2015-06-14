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
	SetAnimations(inAnimations, inCount);
}

void AnimationComponent::SetAnimations(const Animation* inAnimations, uint32_t inCount)
{
	std::vector<Animation> animations;

	for (uint32_t i = 0; i < inCount; ++i)
	{
		animations.push_back( inAnimations[i] );
	}

	SetAnimations(animations);
}

void AnimationComponent::SetAnimations(const std::vector<Animation>& inAnimations)
{
	mAnimations = inAnimations;

	if (mSelectedAnimation >= mAnimations.size())
	{
		mSelectedAnimation = (mAnimations.size() > 0) ? (mAnimations.size() - 1) : 0;
	}
}

void AnimationComponent::SetSelectedAnimation(uint32_t inSelected, bool inResetIt)
{
	assert(inSelected < mAnimations.size());
	mSelectedAnimation = inSelected;
	
	if (inResetIt)
	{
		mAnimations[mSelectedAnimation].Reset();
	}
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
