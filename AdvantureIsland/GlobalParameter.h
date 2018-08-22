#pragma once

#include <cstdlib>
#include <mutex>
#include <set>
#include "PlaceBase.h"
#include "PickupBase.h"
#include "InteractiveBase.h"
#include "land.h"

using namespace std;

enum EWeather
{
	Sunny,
	Rainy,
	Windy
};

template <class T>
class SafeSet
{
private:
	mutex setKeeper;
public:
	set<T> inner;
	SafeSet() :inner(), setKeeper() {}
	void AddElement(T ele);
	void RemoveElement(T ele);
	void _unsafe_add(T ele) { inner.insert(ele); }
	void _unsafe_remove(T ele) { inner.remove(inner.find(ele)); }
	void lockSet() { setKeeper.lock(); }
	void unlockSet() { setKeeper.unlock(); }
	~SafeSet() { for (auto pointer : inner) delete pointer; }
};

template<class T>
inline void SafeSet<T>::AddElement(T ele)
{
	setKeeper.lock();
	inner.insert(ele);
	setKeeper.unlock();
}

template<class T>
inline void SafeSet<T>::RemoveElement(T ele)
{
	setKeeper.lock();
	auto It = inner.find(ele);
	if (It != inner.end()) inner.erase(It);
	setKeeper.unlock();
}

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
	static SafeSet<PlaceBase*> PlaceObjectVec;
	static SafeSet<PickupBase*> PickupObjectSet;
	static SafeSet<class InteractiveBase *> InteractiveObjectSet;
	static vector<vector<Land>> Map;
} GlobalParam;


