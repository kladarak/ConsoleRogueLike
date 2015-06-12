#pragma once

#include <Animations/Animation.h>

class AnimationComponent
{
public:
	AnimationComponent(AnimationComponent&& inRHS);
	AnimationComponent(const AnimationComponent& inRHS);
	AnimationComponent(const Animation& inAnimation);
	AnimationComponent(const Animation* inAnimations, uint32_t inCount);
	~AnimationComponent() { }

	void					SetSelectedAnimation(uint32_t inSelected, bool inResetIt);

	void					Update(float inFrameTime);
	const AsciiMesh&		GetCurrentKeyFrame() const;

private:
	std::vector<Animation>	mAnimations;
	uint32_t				mSelectedAnimation;

};
