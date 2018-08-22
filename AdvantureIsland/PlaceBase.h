#pragma once
class PlaceBase
{
protected:
	char showChar;
	bool bCanTick;
public:
	PlaceBase();
	virtual char GetChar() { return showChar; }
	virtual void Tick(float DeltaSecond) {}
	virtual void PlayerPickUp(class Player * player) {}
	bool GetCanTick() { return bCanTick; }
	~PlaceBase();
};