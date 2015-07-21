#include "Arrow.h"

#include <Renderer/AsciiMesh.h>

#include <GameEntities/Other/ProjectileEntity.h>

namespace Arrow
{

static const AsciiMesh kMeshes[] = 
{
	Fragment(24, ETextYellow),
	Fragment(25, ETextYellow),
	Fragment(26, ETextYellow),
	Fragment(27, ETextYellow),
};

void Create(World& inWorld, const IVec2& inPosition, const IVec2& inDirection)
{
	int meshIndex = 0;

	if		(inDirection.mY < 0) { meshIndex = 0; }
	else if (inDirection.mY > 0) { meshIndex = 1; }
	else if	(inDirection.mX > 0) { meshIndex = 2; }
	else if (inDirection.mX < 0) { meshIndex = 3; }

	ProjectileEntity::Create(inWorld, kMeshes[meshIndex], inPosition, inDirection);
}

}
