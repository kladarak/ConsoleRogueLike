#include "GunTurretEntity.h"

#include <Containers/ContainerMacros.h>

#include <EntityComponentSystem/World/EntityBuilder.h>

#include <EntityComponent/Components/AnimationComponent.h>
#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/ProgramComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <GameEntities/Other/ProjectileEntity.h>

namespace GunTurretEntity
{

static const float	kKeyFrameDuration	= 1.0f; // seconds
static const int	kAttackStrength		= 1;

static const char kFaceUpChar		= gCastUCharToChar(193);
static const char kFaceRightChar	= gCastUCharToChar(195);
static const char kFaceDownChar		= gCastUCharToChar(194);
static const char kFaceLeftChar		= gCastUCharToChar(180);

static const AsciiMesh kKeyFrames[] =
{
	AsciiMesh( Fragment(kFaceUpChar,	ETextMagenta) ),
	AsciiMesh( Fragment(kFaceRightChar, ETextMagenta) ),
	AsciiMesh( Fragment(kFaceDownChar,	ETextMagenta) ),
	AsciiMesh( Fragment(kFaceLeftChar,	ETextMagenta) ),
};

static const Animation kAnimation(kKeyFrames, gElemCount(kKeyFrames), kKeyFrameDuration, Animation::EPlaybackStyle_Loop);

static const AsciiMesh kMissileMesh( Fragment('*', ETextRed) );

class GunTurretComponent
{
public:
	GunTurretComponent() : mTimeElapsed(0.0f) { }
	~GunTurretComponent() { }

	void Update(Entity inThis, float inFrameTime);

private:
	float mTimeElapsed;

};

void GunTurretComponent::Update(Entity inThis, float inFrameTime)
{
	mTimeElapsed += inFrameTime;

	if (mTimeElapsed >= (kKeyFrameDuration + (kKeyFrameDuration*0.5f)))
	{
		mTimeElapsed -= kKeyFrameDuration;

		char image = inThis.GetComponent<RenderableComponent>()->GetMesh().Get(0, 0).mChar;
		
		IVec2 direction = [image] ()
		{
			if (image == kFaceUpChar)
			{
				return IVec2(0, -1);
			}
			else if (image == kFaceRightChar)
			{
				return IVec2(1, 0);
			}
			else if (image == kFaceDownChar)
			{
				return IVec2(0, 1);
			}
			else //if (image == kFaceLeftChar)
			{
				return IVec2(-1, 0);
			}
		} ();

		IVec2 position = inThis.GetComponent<PositionComponent>()->GetPosition() + direction;

		ProjectileEntity::Create(*inThis.GetWorld(), kMissileMesh, position, direction, kAttackStrength);
	}
}

Entity Create(World& inWorld, MessageBroadcaster&, const IVec2& inPos)
{
	auto entity =  EntityBuilder(inWorld)
					.AddComponent<CollisionComponent>( CollisionMesh(0, 0) )
					.AddComponent<PositionComponent>( inPos )
					.AddComponent<RenderableComponent>( kKeyFrames[0] )
					.AddComponent<AnimationComponent>( kAnimation )
					.AddComponent<GunTurretComponent>()
					.Create();

	entity.AddComponent<ProgramComponent>()->RegisterProgram
	(
		[] (const Entity& inThis, float inFrameTime)
		{
			inThis.GetComponent<GunTurretComponent>()->Update(inThis, inFrameTime); 
		} 
	);

	return entity;
}

}
