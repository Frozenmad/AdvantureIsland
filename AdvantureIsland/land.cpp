#include "land.h"
#include "player.h"
#include "PickupBase.h"
#include <Windows.h>

void Land::InnerHelper_SetParameter(ELandType lt, bool step, bool farm, bool spawn, char sc)
{
	LandType = lt;
	bCanStepOn = step;
	bCanFarming = farm;
	bCanSpawnThing = spawn;
	showChar = sc;
	Pickup = NULL;
}

Land::Land(ELandType lt)
{
	LandType = lt;
	switch (lt)
	{
	case Idle:
		InnerHelper_SetParameter(lt, true, false, true, ' ');
		break;
	case Farm:
		InnerHelper_SetParameter(lt, true, true, true, '.');
		break;
	case Destroyed:
		InnerHelper_SetParameter(lt, true, false, false, '`');
		break;
	case Cliff:
		InnerHelper_SetParameter(lt, false, false, false, 'X');
		break;
	case Water:
		InnerHelper_SetParameter(lt, true, false, true, '~');
		break;
	case Default:
		InnerHelper_SetParameter(Cliff, false, false, false, 'X');
		break;
	default:
		InnerHelper_SetParameter(Cliff, false, false, false, 'X');
		break;
	}
}

Land::Land(ELandType lt, bool step, bool farm, bool spawn, char sc)
{
	InnerHelper_SetParameter(lt, step, farm, spawn, sc);
}

void Land::ChangeType(ELandType lt)
{
	LandType = lt;
	switch (lt)
	{
	case Idle:
		InnerHelper_SetParameter(lt, true, false, true, ' ');
		break;
	case Farm:
		InnerHelper_SetParameter(lt, true, true, true, '.');
		break;
	case Destroyed:
		InnerHelper_SetParameter(lt, true, false, false,'`');
		break;
	case Cliff:
		InnerHelper_SetParameter(lt, false, false, false,'X');
		break;
	case Water:
		InnerHelper_SetParameter(lt, true, false, true,'~');
		break;
	case Default:
		InnerHelper_SetParameter(Cliff, false, false, false, 'X');
		break;
	default:
		InnerHelper_SetParameter(Cliff, false, false, false, 'X');
		break;
	}
}

char Land::getChar()
{
	if (HavePickup())
	{
		return Pickup->getPickupChar();
	}

	return showChar;
}

void Land::onStepOnLand(Player * player)
{
	if (HavePickup())
	{
		Pickup->onCollect(player);
		Pickup = NULL;
	}
	switch (LandType)
	{
	case Idle:
		break;
	case Farm:
		break;
	case Destroyed:
		break;
	case Cliff:
		break;
	case Water:
		player->SetPlayerState(EPlayerState::Die);
		break;
	case Default:
		break;
	default:
		break;
	}
}

void Land::AddPickup(PickupBase * InPickup)
{
	Pickup = InPickup;
}

bool Land::HavePickup()
{
	return (Pickup != NULL);
}

int Land::getColor()
{
	switch (LandType)
	{
	case Idle:
		return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
		break;
	case Farm:
		return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		break;
	case Destroyed:
		return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		break;
	case Cliff:
		return 0;
		break;
	case Water:
		return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		break;
	case Default:
		break;
	default:
		break;
	}
	return 0;
}
