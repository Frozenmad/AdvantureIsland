#pragma once
#include "PlaceBase.h"
class Chair :
	public PlaceBase
{
public:
	Chair();
	void PlayerPickUp(class Player * player) override;
	~Chair();
};

