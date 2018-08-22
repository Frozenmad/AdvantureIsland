#include "Wall.h"
#include "player.h"


Wall::Wall()
{
	showChar = 'W';
}

void Wall::PlayerPickUp(Player * player)
{
	player->PlayerInventory.Wall += 1;
}


Wall::~Wall()
{
}
