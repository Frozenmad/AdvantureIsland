#pragma once

#include <cstdlib>
#include <mutex>
#include "Placer.h"

using namespace std;

enum EWeather
{
	Sunny,
	Rainy,
	Windy
};

static class GlobalParameter
{
public:
	static float SickHPDecreaseRate;
	static float HungryStateTurningRate;
	static float HungryRefreshRate;
	static float MaxHealthPoint;
	static float HealthyHPIncreaseRate;
	static EWeather GlobalWeather;
	static mutex OutputMutex;
	static void ChangeWeather();
	static void OutputLock();
	static void OutputUnLock();
} GlobalParam;