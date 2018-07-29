#include "land.h"

void Land::InnerHelper_SetParameter(ELandType lt, bool step, bool farm, bool spawn, char sc)
{
	LandType = lt;
	bCanStepOn = step;
	bCanFarming = farm;
	bCanSpawnThing = spawn;
	showChar = sc;
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
		InnerHelper_SetParameter(lt, true, false, false, 'O');
		break;
	case Cliff:
		InnerHelper_SetParameter(lt, false, false, false, 'X');
		break;
	case Water:
		InnerHelper_SetParameter(lt, false, false, true, '~');
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
		InnerHelper_SetParameter(lt, true, false, false,'O');
		break;
	case Cliff:
		InnerHelper_SetParameter(lt, false, false, false,'X');
		break;
	case Water:
		InnerHelper_SetParameter(lt, false, false, true,'~');
		break;
	case Default:
		InnerHelper_SetParameter(Cliff, false, false, false, 'X');
		break;
	default:
		InnerHelper_SetParameter(Cliff, false, false, false, 'X');
		break;
	}
}
