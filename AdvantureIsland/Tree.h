#pragma once

#include "PickupBase.h"

class Tree : public PickupBase
{
public:
	Tree();
	void onCollect(class Player * player) override;
};

