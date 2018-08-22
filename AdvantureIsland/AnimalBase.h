#pragma once
#include "CommonStructure.h"

class AnimalBase
{
protected:
	Position CurPos;
public:
	AnimalBase(Position initPos);
	virtual void Tick(float DeltaSecond) {}
	~AnimalBase();
};

