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
	"________________      ______________ ",
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
	
	AsciiMesh mesh;

	for (int row = 0; row < ScreenConstants::EMapRows; ++row)
	{
		for (int col = 0; col < ScreenConstants::EMapCols; ++col)
		{
			char c = kRoomMesh[row][col];
			if (c != ' ')
			{
				auto pos = IVec2(col, row);
				mesh.SetCharAtPosition( pos, c );
				collisionComp->SetCollidableAt( pos );
			}
		}
	}

	renderableComp->SetMesh(mesh);
}


}
