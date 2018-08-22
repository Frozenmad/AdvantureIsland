#include "Bed.h"
#include "player.h"


Bed::Bed()
{
	bCanTick = false;
	showChar = 'B';
}

void Bed::PlayerPickUp(Player * player)
{
	player->PlayerInventory.Bed += 1;
}


Bed::~Bed()
{
}
