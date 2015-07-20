#pragma once

#include <Animations/Animation.h>

class AnimationComponent
{
public:
	AnimationComponent() : mSelectedAnimation(0) { }
	AnimationComponent(AnimationComponent&& inRHS);
	AnimationComponent(const AnimationComponent& inRHS);
	AnimationComponent(const Animation& inAnimation);
	AnimationComponent(const Animation* inAnimations, uint32_t inCount);
	~AnimationComponent() { }

	void					SetSelectedAnimation(uint32_t inSelected, bool inResetIt = true);
	void					SetAnimations(const Animation* inAnimations, uint32_t inCount);
	void					SetAnimations(const std::vector<Animation>& inAnimations);
	void					SetAnimation(const Animation& inAnimation);

	bool					IsSelectedAnimationFinished() const;
	void					ResetSelectedAnimation();
	void					Update(float inFrameTime);
	const AsciiMesh&		GetCurrentKeyFrame() const;

private:
	std::vector<Animation>	mAnimations;
	uint32_t				mSelectedAnimation;

};
