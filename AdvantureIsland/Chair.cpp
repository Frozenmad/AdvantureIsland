#include "Chair.h"
#include "player.h"


Chair::Chair()
{
	showChar = 'C';
}

void Chair::PlayerPickUp(Player * player)
{
	player->PlayerInventory.Chair += 1;
}


Chair::~Chair()
{
}
