#pragma once

#include <cstdio>

using namespace std;

class PickupBase
{
protected:
	char ShowChar;
	bool bCanTick;
public:
	PickupBase();
	virtual char getPickupChar() { return ShowChar; }
	virtual void Tick(float DeltaSecond) {}
	virtual void onCollect(class Player * player);
	bool WhetherTick() { return bCanTick; }
};