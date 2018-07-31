#include "player.h"
#include "GlobalParameter.h"
#include <iostream>

Player::Player(float hp, EHealthState health, EHungryState hungry, Position InitPosition, int h, int w, EPlayerState ps)
{
	HealthPoint = hp;
	HealthState = health;
	HungryState = hungry;
	PlayerPosition = InitPosition;
	MapHeight = h;
	MapWidth = w;
	PlayerState = ps;
}

void Player::Tick(float DeltaSecond)
{
	// check whether player is in sick mode
	if (HealthState == Sick)
	{
		HealthPoint = HealthPoint - DeltaSecond / 3600 * GlobalParameter::SickHPDecreaseRate;
	}

	// update starve time
	StarveTime += DeltaSecond;
	if (StarveTime > GlobalParameter::HungryStateTurningRate)
	{
		StarveTime = 0;
		switch (HungryState)
		{
		case Full:
			HungryState = HungryAuto;
			break;
		case Auto:
			HungryState = Hungry;
			break;
		case Hungry:
			HungryState = Starve;
			break;
		case Starve:
			PlayerDie();
			break;
		default:
			break;
		}
	}
}

void Player::PlayerDie()
{
	cout << "You have died!";
}

void Player::SetPlayerPosition(int x, int y)
{
	x < 0 ? PlayerPosition.X = 0 : (x >= MapHeight ? PlayerPosition.X = MapHeight - 1 : PlayerPosition.X = x);
	y < 0 ? PlayerPosition.Y = 0 : (y >= MapWidth ? PlayerPosition.Y = MapHeight - 1 : PlayerPosition.Y = y);
}

void Player::SetPlayerPosition(Position pos)
{
	SetPlayerPosition(pos.X, pos.Y);
}

Position Player::GetPlayerPosition()
{
	return PlayerPosition;
}

SPlayerInventory::SPlayerInventory()
{
	Wood = Stone = Seed = Meat = Vegetable = Trap = Medicine = Wall = Bed = Chair = FireTower = Cook = 0;
}
