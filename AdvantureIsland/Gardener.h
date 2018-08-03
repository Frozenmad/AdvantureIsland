#pragma once
#include "InteractiveBase.h"
class Gardener : public InteractiveBase
{
public:
	Gardener();
	void ProcessQuery(class Player * player, COORD showPos);
	~Gardener();
};

