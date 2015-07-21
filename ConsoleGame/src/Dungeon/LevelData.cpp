#include "LevelData.h"

#include <Containers/ContainerMacros.h>

#include <GameEntities/Monsters/WormMonster.h>
#include <GameEntities/Monsters/WallSparkMonster.h>
#include <GameEntities/Monsters/ArcherMonster.h>
#include <GameEntities/Monsters/SpinnerEntity.h>
#include <GameEntities/Monsters/CaterpillarMonster.h>
#include <GameEntities/Monsters/GunTurretEntity.h>

#include <GameEntities/ItemEntity.h>

#include <Inventory/Items/Sword.h>
#include <Inventory/Items/Bow.h>
#include <Inventory/Items/Shield.h>
#include <Inventory/Items/Bomb.h>

// Macros defined to make level data easier to read.
// JSON might be more of an approriate format for this.

#define START_LEVEL_DATA				{
#define ROOM_COUNT(n)						n,
#define MONEY_COUNT(n)						n,
#define	ITEM_FACTORY(item)					&ItemEntity::Create< item >,
#define	NO_ITEM_FACTORY						nullptr,
#define START_MONSTER_FACTORIES				{
#define ADD_MONSTER_FACTORY(n, funcPtr)			{ n, funcPtr },
#define END_MONSTER_FACTORIES				},
#define END_LEVEL_DATA					},

#define WORM_MONSTER		&WormMonster::Create
#define SPARK_MONSTER		&WallSparkMonster::Create
#define ARCHER_MONSTER		&ArcherMonster::Create
#define SPINNER_OBJECT		&SpinnerEntity::Create
#define CATERPILLAR_MONSTER	&CaterpillarMonster::Create
#define GUN_TURRET_OBJECT	&GunTurretEntity::Create

const LevelData kLevelData[]
{
	START_LEVEL_DATA
		ROOM_COUNT(1)
		MONEY_COUNT(1)
		ITEM_FACTORY( Sword )
		START_MONSTER_FACTORIES
			ADD_MONSTER_FACTORY(1, CATERPILLAR_MONSTER)
			ADD_MONSTER_FACTORY(1, GUN_TURRET_OBJECT)
		END_MONSTER_FACTORIES
	END_LEVEL_DATA

	START_LEVEL_DATA
		ROOM_COUNT(3)
		MONEY_COUNT(3)
		ITEM_FACTORY( Shield )
		START_MONSTER_FACTORIES
			ADD_MONSTER_FACTORY(3, WORM_MONSTER)
			ADD_MONSTER_FACTORY(3, SPINNER_OBJECT)
		END_MONSTER_FACTORIES
	END_LEVEL_DATA

	START_LEVEL_DATA
		ROOM_COUNT(5)
		MONEY_COUNT(5)
		ITEM_FACTORY( Bow )
		START_MONSTER_FACTORIES
			ADD_MONSTER_FACTORY(4, WORM_MONSTER)
			ADD_MONSTER_FACTORY(4, SPARK_MONSTER)
			ADD_MONSTER_FACTORY(4, ARCHER_MONSTER)
		END_MONSTER_FACTORIES
	END_LEVEL_DATA

	START_LEVEL_DATA
		ROOM_COUNT(5)
		MONEY_COUNT(5)
		ITEM_FACTORY( Bomb )
		START_MONSTER_FACTORIES
			ADD_MONSTER_FACTORY(4, WORM_MONSTER)
			ADD_MONSTER_FACTORY(4, SPARK_MONSTER)
			ADD_MONSTER_FACTORY(4, ARCHER_MONSTER)
		END_MONSTER_FACTORIES
	END_LEVEL_DATA

	START_LEVEL_DATA
		ROOM_COUNT(5)
		MONEY_COUNT(5)
		NO_ITEM_FACTORY
		START_MONSTER_FACTORIES
			ADD_MONSTER_FACTORY(4, WORM_MONSTER)
			ADD_MONSTER_FACTORY(4, SPARK_MONSTER)
			ADD_MONSTER_FACTORY(4, ARCHER_MONSTER)
		END_MONSTER_FACTORIES
	END_LEVEL_DATA
};

const uint32_t kLevelDataCount = gElemCount(kLevelData);
