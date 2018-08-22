#pragma once
#include "PlaceBase.h"
class Trap :
	public PlaceBase
{
public:
	Trap();
	void PlayerPickUp(class Player * player) override;
	~Trap();
};

