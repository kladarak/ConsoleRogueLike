#pragma once

#include <Renderer/AsciiMesh.h>

class AnimationComponent
{
public:
	AnimationComponent();
	AnimationComponent(AnimationComponent&& inRHS);
	AnimationComponent(const AnimationComponent& inRHS);
	AnimationComponent(const AsciiMesh* inMeshes, int inCount, float inKeyFrameDuration);

	void					Update(float inFrameTime);
	const AsciiMesh&		GetCurrentKeyFrame() const;


private:
	std::vector<AsciiMesh>	mKeyFrames;
	int						mCurrentKeyFrame;
	float					mKeyFrameDuration;
	float					mRunningTime;

};
