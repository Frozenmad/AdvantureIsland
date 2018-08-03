#pragma once
#include "CommonStructure.h"
#include <mutex>

class InteractiveBase
{
protected:

	ConsoleMap * InteractiveMap;
	char ShowChar;

public:

	InteractiveBase();
	virtual char GetChar() { return ShowChar; }
	virtual void ProcessQuery(class Player * player, COORD showPos) {}
	~InteractiveBase();
};

