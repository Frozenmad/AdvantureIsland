#pragma once

#include <cstdlib>
#include <vector>
#include "CommonStructure.h"

using namespace std;

enum EHealthState
{
	Healthy,
	Auto,
	Sick
};

enum EHungryState
{
	Full,
	HungryAuto,
	Hungry,
	Starve
};

class Player
{
private:
	float HealthPoint;
	EHealthState HealthState;
	EHungryState HungryState;
	float StarveTime;
	Position PlayerPosition;
	int MapHeight;
	int MapWidth;

public:
	Player(float hp, EHealthState heath, EHungryState hungry, Position InitPosition, int h = 100, int w = 100);
	void Tick(float DeltaSecond);
	void PlayerDie();
	void SetPlayerPosition(int x, int y);
	void SetPlayerPosition(Position pos);
	Position GetPlayerPosition();
};