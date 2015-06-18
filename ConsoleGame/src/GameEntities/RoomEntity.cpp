#include "RoomEntity.h"

#include <EntityComponentSystem/World/World.h>

#include <EntityComponent/Components/CollisionComponent.h>
#include <EntityComponent/Components/PositionComponent.h>
#include <EntityComponent/Components/RenderableComponent.h>

#include "ScreenConstants.h"

namespace RoomEntity
{

static const char kRoomMesh[ScreenConstants::EMapRows][ScreenConstants::EMapCols] =
{
	" _______________      ______________ ",
	"|               |    |              |",
	"|                                   |",
	"|                                   |",
	"|                                   |",
	"|_                                 _|",
	"                                     ",
	"                                     ",
	" _                                 _ ",
	"|                                   |",
	"|                                   |",
	"|                                   |",
	"|                                   |",
	"|_______________|    |______________|",
};


void Create(World& inWorld)
{
	Entity entity = inWorld.CreateEntity();

	auto positionComp	= entity.AddComponent<PositionComponent>();
	auto collisionComp	= entity.AddComponent<CollisionComponent>();
	auto renderableComp	= entity.AddComponent<RenderableComponent>();

	positionComp->SetPosition( IVec2(0, 0) );
	
	AsciiMesh		renderMesh;
	CollisionMesh	collisionMesh;

	for (int row = 0; row < ScreenConstants::EMapRows; ++row)
	{
		for (int col = 0; col < ScreenConstants::EMapCols; ++col)
		{
			char c = kRoomMesh[row][col];
			if (c != ' ')
			{
				renderMesh.SetCharAtPosition( col, row, c );
				collisionMesh.SetCollidableAt( col, row );
			}
		}
	}

	renderableComp->SetMesh(renderMesh);
	collisionComp->SetDefaultCollisionMesh(collisionMesh);
}

}
