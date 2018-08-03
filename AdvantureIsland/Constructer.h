#pragma once
#include "InteractiveBase.h"
class Constructer : public InteractiveBase
{
public:
	Constructer();
	void ProcessQuery(Player * player, COORD showPos) override;
	~Constructer();
};

