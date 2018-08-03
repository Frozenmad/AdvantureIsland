#pragma once
#include "CommonStructure.h"
#include "land.h"

class Placer
{
protected:

	class ConsoleMap * InteractiveCons;

public:

	Placer();
	void Place(vector<vector<Land>> &Map, class Player * player, COORD showPos);
	~Placer();

};

