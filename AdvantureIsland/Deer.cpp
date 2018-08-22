#include "Deer.h"
#include "GlobalParameter.h"

Deer::Deer(Position initPos) : AnimalBase(initPos)
{
	PassSecond = MoveSecond = StopSecond = 0;
	state = 0;
	direction = 0;
}

void Deer::Tick(float DeltaSecond)
{
	PassSecond += DeltaSecond;
	switch (state)
	{
	case 0:
		if (PassSecond > MoveSecond)
		{
			// This deer should move to next position.
			switch (direction)
			{
			case 0:
				Position Target = CurPos + Position(0, -1);
				if (GlobalParameter::Map[Target.X][Target.Y].CanStepOn())
				{

				}
			}
		}
	case 1:
		if (PassSecond > StopSecond)
		{
			state = 0;
			PassSecond = 0;
			break;
		}
	default:
		break;
	}
}


Deer::~Deer()
{
}
