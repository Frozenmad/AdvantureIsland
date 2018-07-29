#include "player.h"
#include "GlobalParameter.h"
#include <iostream>

Player::Player(float hp, EHealthState health, EHungryState hungry, vector<int> InitPosition)
{
	HealthPoint = hp;
	HealthState = health;
	HungryState = hungry;
	PlayerPosition = InitPosition;
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
