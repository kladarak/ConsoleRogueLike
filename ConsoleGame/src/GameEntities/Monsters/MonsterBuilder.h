#pragma once

#include <EntityComponentSystem/World/World.h>
#include <Animations/Animation.h>
#include <Renderer/AsciiMesh.h>

class MessageBroadcaster;
struct IVec2;

class MonsterBuilder
{
public:
	MonsterBuilder(World& inWorld, MessageBroadcaster* inMsgBroadcaster);

	MonsterBuilder& operator=(const MonsterBuilder&) = delete;

	MonsterBuilder&		SetAnimation(const Animation& inAnimation);
	MonsterBuilder&		SetPosition(const IVec2& inPosition);
	MonsterBuilder&		SetRenderable(const AsciiMesh& inAsciiMesh);

	MonsterBuilder&		SetInvulnerable();

	Entity				Create();

private:
	enum EComponents
	{
		EAnimation  = 1 << 0,
	};

	World&				mWorld;
	MessageBroadcaster* mMsgBroadcaster;

	Animation			mAnimation;
	AsciiMesh			mRenderMesh;
	IVec2				mPosition;

	uint32_t			mOptionalComponents;
	bool				mCanBeKilled;
};
