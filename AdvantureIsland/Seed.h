#pragma once
#include "PickupBase.h"
class Seed : public PickupBase
{
protected:
	// The Seed state, when the seed on the island 
	int state;
	float PassTime;
	float GrowTimeLmt;
public:
	Seed(float gtl = 3600 * 24 * 2);
	void onCollect(class Player * player) override;
	char getPickupChar() override;
	void Tick(float DeltaSecond) override;
	~Seed();
};

