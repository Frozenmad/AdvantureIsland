#pragma once
#include "PlaceBase.h"
class Bed :
	public PlaceBase
{
public:
	Bed();
	void PlayerPickUp(class Player * player) override;
	~Bed();
};

