#include "OpenDoor.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include <Containers/ContainerMacros.h>

#include "DoorConstants.h"

namespace OpenDoor
{
	
namespace FaceUpDown
{

	static const char			kRenderMeshData[EDoorSize_Width+1] = { "|     |" };  //+1 for terminating character...
	static const AsciiMesh		kRenderMesh(kRenderMeshData, 7, 1);
	
	static const bool			kCollisionMeshData[] = { true, false, false, false, false, false, true };
	static const CollisionMesh	kCollisionMesh(kCollisionMeshData,	7, 1);
}

namespace FaceLeftRight
{
	static const char kRenderMeshData[EDoorSize_Height] = 
	{
		'_',
		' ',
		' ',
		' ',
		gCastUCharToChar(238),
	};
	
	static const AsciiMesh		kRenderMesh(kRenderMeshData, 1, 5);
	
	static const bool			kCollisionMeshData[]	= { true, false, false, false, true };
	static const CollisionMesh	kCollisionMesh(kCollisionMeshData,	1, 5);
}

void Create(World& inWorld, const IVec2& inPosition, EOrientation inOrientation)
{
	auto entity = inWorld.CreateEntity();

	bool	isLeftRight = (inOrientation == EOrientation_FaceLeft) || (inOrientation == EOrientation_FaceRight);
	
	auto&	renderMesh		= isLeftRight ? FaceLeftRight::kRenderMesh		: FaceUpDown::kRenderMesh;
	auto&	collisionMesh	= isLeftRight ? FaceLeftRight::kCollisionMesh	: FaceUpDown::kCollisionMesh;
	
	entity.AddComponent<PositionComponent>(inPosition);
	entity.AddComponent<RenderableComponent>(renderMesh);
	entity.AddComponent<CollisionComponent>(collisionMesh);
}

}
