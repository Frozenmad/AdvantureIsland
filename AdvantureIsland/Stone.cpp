#include "Stone.h"
#include "player.h"


Stone::Stone()
{
	ShowChar = '*';
}

void Stone::onCollect(Player * player)
{
	player->PlayerInventory.Stone += 1;
}


Stone::~Stone()
{
}
