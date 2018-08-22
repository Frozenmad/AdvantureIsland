#pragma once
#include "AnimalBase.h"


/* Deer class 
This kind of animal will go until it hit something, and will turn to next direction.
*/
class Deer :
	public AnimalBase
{
	float PassSecond;
	float MoveSecond;
	float StopSecond;
	
	/*
	the state of this animal.
	0 --- move
	1 --- stop
	*/
	int state;

	/*
	the current direction of this animal
	0 --- left
	1 --- up
	2 --- right
	3 --- down
	*/
	int direction;

public:
	Deer(Position initPos);
	void Tick(float DeltaSecond) override;
	~Deer();
};

