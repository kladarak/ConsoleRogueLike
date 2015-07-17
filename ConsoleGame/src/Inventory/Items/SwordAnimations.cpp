#include "SwordAnimations.h"

#include <Containers/ContainerMacros.h>
#include <GameEntities/Player/PlayerConstants.h>

namespace SwordAnimations
{

static const float		kSwordKeyFrameDuration	= 0.05f;
static const Fragment	kPlayerSprite			= PlayerConstants::kSprite;

namespace SwordFaceUp
{
	static const Fragment kSwordFaceUp0[]	= {	'_',
												kPlayerSprite };

	static const Fragment kSwordFaceUp1[]	= {	'\\',
												kPlayerSprite };

	static const Fragment kSwordFaceUp2[]	= {	' ',			'|',
												kPlayerSprite,	' ' };

	static const AsciiMesh kSwordFaceUpKeyFrames[] =
	{
		AsciiMesh( kSwordFaceUp0,	1, 2, IVec2( 0, -1) ),
		AsciiMesh( kSwordFaceUp1,	1, 2, IVec2( 0, -1) ),
		AsciiMesh( kSwordFaceUp2,	2, 2, IVec2( 0, -1) ),
	};

	static const Animation kAnimation(	kSwordFaceUpKeyFrames, 
										gElemCount(kSwordFaceUpKeyFrames), 
										kSwordKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace SwordFaceDown
{
	static const Fragment kSwordFaceDown0[]	= {	kPlayerSprite,
												'-' };

	static const Fragment kSwordFaceDown1[]	= {	kPlayerSprite,
												'\\' };

	static const Fragment kSwordFaceDown2[]	= {	' ', kPlayerSprite,
												'|', ' ' };

	static const AsciiMesh kSwordFaceDownKeyFrames[] =
	{
		AsciiMesh( kSwordFaceDown0,	1, 2, IVec2( 0, 0) ),
		AsciiMesh( kSwordFaceDown1,	1, 2, IVec2( 0, 0) ),
		AsciiMesh( kSwordFaceDown2,	2, 2, IVec2(-1, 0) ),
	};

	static const Animation kAnimation(	kSwordFaceDownKeyFrames, 
										gElemCount(kSwordFaceDownKeyFrames), 
										kSwordKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace SwordFaceLeft
{
	static const Fragment kSwordFaceLeft0[]	= { '|', kPlayerSprite };
	static const Fragment kSwordFaceLeft1[]	= { '/', kPlayerSprite };
	static const Fragment kSwordFaceLeft2[]	= { gCastUCharToChar(238), kPlayerSprite }; // should be "overscore" / "macron"

	static const AsciiMesh kSwordFaceLeftKeyFrames[] =
	{
		AsciiMesh( kSwordFaceLeft0,	2, 1, IVec2(-1, 0) ),
		AsciiMesh( kSwordFaceLeft1,	2, 1, IVec2(-1, 0) ),
		AsciiMesh( kSwordFaceLeft2,	2, 1, IVec2(-1, 0) ),
	};

	static const Animation kAnimation(	kSwordFaceLeftKeyFrames, 
										gElemCount(kSwordFaceLeftKeyFrames), 
										kSwordKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace SwordFaceRight
{
	static const Fragment kSwordFaceRight0[] = { kPlayerSprite, '|' };
	static const Fragment kSwordFaceRight1[] = { kPlayerSprite, '/' };
	static const Fragment kSwordFaceRight2[] = { kPlayerSprite, '_' };

	static const AsciiMesh kSwordFaceRightKeyFrames[] =
	{
		AsciiMesh( kSwordFaceRight0, 2, 1, IVec2(0, 0) ),
		AsciiMesh( kSwordFaceRight1, 2, 1, IVec2(0, 0) ),
		AsciiMesh( kSwordFaceRight2, 2, 1, IVec2(0, 0) ),
	};

	static const Animation kAnimation(	kSwordFaceRightKeyFrames, 
										gElemCount(kSwordFaceRightKeyFrames), 
										kSwordKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

static const Animation kAnimations[] =
{
	SwordFaceUp::kAnimation,
	SwordFaceDown::kAnimation,
	SwordFaceLeft::kAnimation,
	SwordFaceRight::kAnimation,
};

std::vector<Animation> Generate()
{
	std::vector<Animation> animations;

	for (size_t i = 0; i < gElemCount(kAnimations); ++i)
	{
		animations.push_back( kAnimations[i] );
	}

	return animations;
}

}