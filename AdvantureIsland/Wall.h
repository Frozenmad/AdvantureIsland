#pragma once
#include "PlaceBase.h"
class Wall :
	public PlaceBase
{
public:
	Wall();
	void PlayerPickUp(class Player * player) override;
	~Wall();
};

