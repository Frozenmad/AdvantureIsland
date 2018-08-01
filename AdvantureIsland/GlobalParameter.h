#pragma once

#include <cstdlib>

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
	static void ChangeWeather();
};