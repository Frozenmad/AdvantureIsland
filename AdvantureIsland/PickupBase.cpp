#include "PickupBase.h"
#include "player.h"

PickupBase::PickupBase()
{
	ShowChar = ' ';
	bCanTick = false;
}

void PickupBase::onCollect(Player * player)
{
}
