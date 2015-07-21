#include "SpinnerEntity.h"

#include "MonsterBuilder.h"

namespace SpinnerEntity
{

static const float kStateChangeRate = 0.05f; // seconds

static const AsciiMesh kSpinnerFrames[] =
{
	AsciiMesh( Fragment('|', ETextBlue) ),
	AsciiMesh( Fragment('/', ETextBlue) ),
	AsciiMesh( Fragment('-', ETextBlue) ),
	AsciiMesh( Fragment('\\', ETextBlue) ),
};

static const Animation kSpinnerAnimation(kSpinnerFrames, sizeof(kSpinnerFrames)/sizeof(AsciiMesh), kStateChangeRate, Animation::EPlaybackStyle_Loop);

Entity Create(World& inWorld, MessageBroadcaster& inMsgBroadcaster, const IVec2& inPos)
{
	return MonsterBuilder(inWorld, &inMsgBroadcaster)
			.SetPosition(inPos)
			.SetRenderable( kSpinnerFrames[0] )
			.SetAnimation(kSpinnerAnimation)
			.SetInvulnerable()
			.Create();
}

}
