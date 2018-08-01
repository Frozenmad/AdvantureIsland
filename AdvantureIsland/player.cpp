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
	StarveTime = 0;
}

void Player::Tick(float DeltaSecond)
{
	// check whether player is in sick mode
	if (HealthState == Sick)
	{
		HealthPoint = HealthPoint - DeltaSecond / 3600 * GlobalParameter::SickHPDecreaseRate;
	}

	else if (HealthState == EHealthState::Healthy && HealthPoint < GlobalParameter::MaxHealthPoint)
	{
		HealthPoint = HealthPoint + DeltaSecond / 3600 * GlobalParameter::HealthyHPIncreaseRate;
		if (HealthPoint > GlobalParameter::MaxHealthPoint)
		{
			HealthPoint = GlobalParameter::MaxHealthPoint;
		}
	}

	// update starve time
	StarveTime += DeltaSecond;
	if (StarveTime > GlobalParameter::HungryStateTurningRate)
	{
		StarveTime = 0;
		switch (HungryState)
		{
		case EHungryState::Full:
			HungryState = HungryAuto;
			break;
		case EHungryState::HungryAuto:
			HungryState = Hungry;
			break;
		case EHungryState::Hungry:
			HungryState = Starve;
			break;
		case EHungryState::Starve:
			PlayerDie();
			break;
		default:
			break;
		}
	}
}

void Player::PlayerDie()
{
	PlayerState = EPlayerState::Die;
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

void Player::TakeFood()
{
	PlayerInventory.Food -= 1;
	StarveTime = StarveTime - GlobalParameter::HungryRefreshRate;
	if (StarveTime < 0)
	{
		StarveTime = 0;
		switch (HungryState)
		{
		case Full:
			break;
		case HungryAuto:
			HungryState = Full;
			break;
		case Hungry:
			HungryState = HungryAuto;
			break;
		case Starve:
			HungryState = Hungry;
			break;
		default:
			break;
		}
	}
}

void Player::TakePill()
{
	PlayerInventory.Medicine -= 1;
	switch (HealthState)
	{
	case Healthy:
		break;
	case Auto:
		HealthState = Healthy;
		break;
	case Sick:
		HealthState = Auto;
		break;
	default:
		break;
	}
}

bool Player::CanTakeFood()
{
	return PlayerInventory.Food > 0;
}

bool Player::CanTakePill()
{
	return (HealthState != EHealthState::Healthy) && (PlayerInventory.Medicine > 0);
}

void Player::PlayerSick()
{
	switch (HealthState)
	{
	case Healthy:
		HealthState = EHealthState::Sick;
		break;
	case Auto:
		HealthState = EHealthState::Sick;
		break;
	case Sick:
		break;
	default:
		break;
	}
}

Position Player::GetPlayerPosition()
{
	return PlayerPosition;
}

SPlayerInventory::SPlayerInventory()
{
	Wood = Stone = Seed = Meat = Vegetable = Trap = Medicine = Wall = Bed = Chair = FireTower = Cook = Food = 0;
}
