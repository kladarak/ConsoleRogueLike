#include "BowAnimations.h"

#include <Containers/ContainerMacros.h>

namespace BowAnimations
{

static const float kBowKeyFrameDuration = 0.1f;

namespace BowFaceUp
{
	static const char kBowFaceUp0[]	= {	'+',
										'O' };
	static const char kBowFaceUp1[]	= {	'-',
										'O' };

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
	static const char kBowFaceDown0[]	= {	'O',
											'+' };
	static const char kBowFaceDown1[]	= {	'O',
											'-' };

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
	static const char kBowFaceLeft0[]	= { '+','O' };
	static const char kBowFaceLeft1[]	= { '|','O' };

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
	static const char kBowFaceRight0[] = { 'O','+' };
	static const char kBowFaceRight1[] = { 'O','|' };

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