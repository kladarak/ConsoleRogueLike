#include "BowAnimations.h"

#include <Containers/ContainerMacros.h>
#include <GameEntities/Player/PlayerConstants.h>

namespace BowAnimations
{

static const float		kBowKeyFrameDuration	= 0.1f;
static const Fragment	kPlayerSprite			= PlayerConstants::kSprite;

static const Fragment	kBowWithArrowFragment(gCastUCharToChar(197), ETextDarkYellow);
static const Fragment	kBowOnlyVertiFragment(gCastUCharToChar(179), ETextDarkYellow);
static const Fragment	kBowOnlyHorizFragment(gCastUCharToChar(196), ETextDarkYellow);


namespace BowFaceUp
{
	static const Fragment kBowFaceUp0[]	= {	kBowWithArrowFragment,
											kPlayerSprite };
	static const Fragment kBowFaceUp1[]	= {	kBowOnlyHorizFragment,
											kPlayerSprite };

	static const AsciiMesh kBowFaceUpKeyFrames[] =
	{
		AsciiMesh( kBowFaceUp0,	1, 2, IVec2( 0, -1) ),
		AsciiMesh( kBowFaceUp1,	1, 2, IVec2( 0, -1) ),
	};

	static const Animation kAnimation(	kBowFaceUpKeyFrames, 
										gElemCount(kBowFaceUpKeyFrames), 
										kBowKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace BowFaceDown
{
	static const Fragment kBowFaceDown0[]	= {	kPlayerSprite,
												kBowWithArrowFragment };
	static const Fragment kBowFaceDown1[]	= {	kPlayerSprite,
												kBowOnlyHorizFragment };

	static const AsciiMesh kBowFaceDownKeyFrames[] =
	{
		AsciiMesh( kBowFaceDown0,	1, 2, IVec2( 0, 0) ),
		AsciiMesh( kBowFaceDown1,	1, 2, IVec2( 0, 0) ),
	};

	static const Animation kAnimation(	kBowFaceDownKeyFrames, 
										gElemCount(kBowFaceDownKeyFrames), 
										kBowKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace BowFaceLeft
{
	static const Fragment kBowFaceLeft0[]	= { kBowWithArrowFragment, kPlayerSprite };
	static const Fragment kBowFaceLeft1[]	= { kBowOnlyVertiFragment, kPlayerSprite };

	static const AsciiMesh kBowFaceLeftKeyFrames[] =
	{
		AsciiMesh( kBowFaceLeft0,	2, 1, IVec2(-1, 0) ),
		AsciiMesh( kBowFaceLeft1,	2, 1, IVec2(-1, 0) ),
	};

	static const Animation kAnimation(	kBowFaceLeftKeyFrames, 
										gElemCount(kBowFaceLeftKeyFrames), 
										kBowKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

namespace BowFaceRight
{
	static const Fragment kBowFaceRight0[] = { kPlayerSprite, kBowWithArrowFragment };
	static const Fragment kBowFaceRight1[] = { kPlayerSprite, kBowOnlyVertiFragment };

	static const AsciiMesh kBowFaceRightKeyFrames[] =
	{
		AsciiMesh( kBowFaceRight0, 2, 1, IVec2(0, 0) ),
		AsciiMesh( kBowFaceRight1, 2, 1, IVec2(0, 0) ),
	};

	static const Animation kAnimation(	kBowFaceRightKeyFrames, 
										gElemCount(kBowFaceRightKeyFrames), 
										kBowKeyFrameDuration, 
										Animation::EPlaybackStyle_Once );
}

//--------------------------------------------------------------------

static const Animation kAnimations[] =
{
	BowFaceUp::kAnimation,
	BowFaceDown::kAnimation,
	BowFaceLeft::kAnimation,
	BowFaceRight::kAnimation,
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