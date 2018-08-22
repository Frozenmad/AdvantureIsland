#include "Trap.h"
#include "player.h"


Trap::Trap()
{
	showChar = 'x';
}

void Trap::PlayerPickUp(Player * player)
{
	player->PlayerInventory.Trap += 1;
}


Trap::~Trap()
{
}
