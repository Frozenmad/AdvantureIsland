#include "GlobalParameter.h"
#include <cstdlib>

float GlobalParameter::HungryRefreshRate = 3600 * 4;
float GlobalParameter::HungryStateTurningRate = 3600 * 8;
float GlobalParameter::SickHPDecreaseRate = 5;
float GlobalParameter::MaxHealthPoint = 100.0;
float GlobalParameter::HealthyHPIncreaseRate = 5;
EWeather GlobalParameter::GlobalWeather = EWeather::Sunny;
mutex GlobalParameter::OutputMutex;
SafeSet<PlaceBase *> GlobalParameter::PlaceObjectVec;
SafeSet<PickupBase *> GlobalParameter::PickupObjectSet;
SafeSet<InteractiveBase *> GlobalParameter::InteractiveObjectSet;
vector<vector<Land>> GlobalParameter::Map;

void GlobalParameter::ChangeWeather()
{
	double rate = rand() / (RAND_MAX + 1.0);
	if (rate < 0.5)
	{
		GlobalWeather = EWeather::Sunny;
	}
	else if (rate < 0.8)
	{
		GlobalWeather = EWeather::Windy;
	}
	else
	{
		GlobalWeather = EWeather::Rainy;
	}
}

void GlobalParameter::OutputLock()
{
	OutputMutex.lock();
}

void GlobalParameter::OutputUnLock()
{
	OutputMutex.unlock();
}