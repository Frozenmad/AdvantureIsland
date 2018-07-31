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

enum EPlayerState
{
	Live,
	Die
};

struct SPlayerInventory
{
	int Wood;
	int Stone;
	int Seed;
	int Meat;
	int Vegetable;
	int Trap;
	int Medicine;
	int Wall;
	int Bed;
	int Chair;
	int FireTower;
	int Cook;
	SPlayerInventory();
	SPlayerInventory(int w, int st, int se, int mea, int ve, int t, int me, int wa, int be, int ch, int fi, int co) :
		Wood(w), Stone(st), Seed(se), Meat(mea), Vegetable(ve), Trap(t), Medicine(me), Wall(wa), Bed(be), Chair(ch), FireTower(fi), Cook(co) {}
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
	EPlayerState PlayerState;

public:
	SPlayerInventory PlayerInventory;
	Player(float hp, EHealthState heath, EHungryState hungry, Position InitPosition, int h = 100, int w = 100, EPlayerState ps = Live);
	void Tick(float DeltaSecond);
	void PlayerDie();
	void SetPlayerPosition(int x, int y);
	void SetPlayerPosition(Position pos);
	Position GetPlayerPosition();
	EPlayerState GetPlayerState() { return PlayerState; }
	EPlayerState SetPlayerState(EPlayerState TargetState) { PlayerState = TargetState; return TargetState; }
};