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
	class InteractiveBase * InteractObject;
	class PlaceBase * PlaceObject;

public:
	Land(ELandType lt);
	Land(ELandType lt, bool step, bool farm, bool spawn, char sc);
	void ChangeType(ELandType lt);
	bool CanStepOn();
	bool CanFarming() { return bCanFarming; }
	bool CanSpawnThing() { return bCanSpawnThing; }
	char getChar();
	void onStepOnLand(class Player * player);
	void AddPickup(PickupBase * InPickup);
	bool HavePickup();
	int getColor();
	void AddInteractive(InteractiveBase * ib);
	void AddPlaceObject(PlaceBase * pb);
	InteractiveBase * getInteractiveBase() { return InteractObject; }
	PlaceBase * getPlaceBase() { return PlaceObject; }
	void PickupPlaceBase(Player * player);
	bool HavePlaceBase();
	bool HaveInteractive();
};