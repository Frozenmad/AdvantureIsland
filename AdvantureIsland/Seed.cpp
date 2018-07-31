#include "Seed.h"
#include "player.h"

Seed::Seed(float gtl)
{
	bCanTick = true;
	ShowChar = 's';
	PassTime = 0;
	state = 1;
	GrowTimeLmt = gtl;
}

void Seed::onCollect(Player * player)
{
	if (state == 1) 
	{
		player->PlayerInventory.Seed += 1;
	}
	else if (state == 2)
	{
		player->PlayerInventory.Vegetable += 1;
	}
}

char Seed::getPickupChar()
{
	if (state == 1)
	{
		return '\"';
	}
	else
	{
		return '#';
	}
}

void Seed::Tick(float DeltaSecond)
{
	if (state == 2)
	{
		bCanTick = false;
		return;
	}

	PassTime += DeltaSecond;

	if (PassTime > GrowTimeLmt)
	{
		state = 2;
	}
}


Seed::~Seed()
{
}