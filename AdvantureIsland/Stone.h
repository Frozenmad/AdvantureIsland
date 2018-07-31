#pragma once
#include "PickupBase.h"
class Stone : public PickupBase
{
public:
	Stone();
	void onCollect(class Player * player) override;
	~Stone();
};

