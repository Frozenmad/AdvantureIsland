#pragma once
#include "InteractiveBase.h"
class Cooker : public InteractiveBase
{
public:
	Cooker();
	void ProcessQuery(class Player * player, COORD showPos) override;
	~Cooker();
};

