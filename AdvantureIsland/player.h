#pragma once

#include <cstdlib>
#include <vector>

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
	vector<int> PlayerPosition;
	float StarveTime;

public:
	Player(float hp, EHealthState heath, EHungryState hungry, vector<int> InitPosition);
	void Tick(float DeltaSecond);
	void PlayerDie();
};