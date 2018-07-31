#pragma once

#include <cstdlib>

using namespace std;

enum ELandType
{
	Idle,
	Farm,
	Destroyed,
	Cliff,
	Water,
	Default
};

class Land
{
private:
	ELandType LandType;
	bool bCanStepOn;
	bool bCanFarming;
	bool bCanSpawnThing;
	char showChar;
	void InnerHelper_SetParameter(ELandType lt, bool step, bool farm, bool spawn, char sc);
	class PickupBase * Pickup;

public:
	Land(ELandType lt);
	Land(ELandType lt, bool step, bool farm, bool spawn, char sc);
	void ChangeType(ELandType lt);
	bool CanStepOn() { return bCanStepOn; }
	bool CanFarming() { return bCanFarming; }
	bool CanSpawnThing() { return bCanSpawnThing; }
	char getChar();
	void onStepOnLand(class Player * player);
	void AddPickup(PickupBase * InPickup);
	bool HavePickup();
};