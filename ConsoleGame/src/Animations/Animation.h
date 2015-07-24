#pragma once

#include <stdint.h>
#include <Renderer/AsciiMesh.h>

class Animation
{
public:
	enum EPlaybackStyle
	{
		EPlaybackStyle_Once,
		EPlaybackStyle_Loop
	};

	Animation();
	Animation(Animation&& inRHS);
	Animation(const Animation& inRHS);
	Animation(const AsciiMesh* inMeshes, uint32_t inCount, float inKeyFrameDuration, EPlaybackStyle inPlaybackStyle);
	Animation(const std::vector<AsciiMesh>& inMeshes, float inKeyFrameDuration, EPlaybackStyle inPlaybackStyle);

	void					Update(float inFrameTime);
	const AsciiMesh&		GetCurrentKeyFrame() const;
	void					Reset();
	bool					IsFinished() const;

private:
	std::vector<AsciiMesh>	mKeyFrames;
	uint32_t				mCurrentKeyFrame;
	float					mKeyFrameDuration;
	float					mRunningTime;
	EPlaybackStyle			mPlaybackStyle;
	bool					mIsFinished;
};
