

#include<functional>

#include"hauntedcity.h"
#include"autofactory.h"
#include"observer.h"

autoFactory<Building>::registerClass<HauntedCity> regForHauntedCity("hauntedcity");

extern Observer<std::function<void(int, int&)>> hauntedCityObserver;

HauntedCity::HauntedCity() :Building(2, BuildingType::SPECIAL, "hauntedcity") {
}

int HauntedCity::getSymbolType() {
	return 0;
}

void hauntedCityObserverFunc(int bel, int idx, int& type_set) {
	if (bel != idx)return;
	if (!(type_set & BuildingType::COMMERCE)) { type_set |= BuildingType::COMMERCE; return; }
	if (!(type_set & BuildingType::MILITARY)) { type_set |= BuildingType::MILITARY; return; }
	if (!(type_set & BuildingType::NOBILITY)) { type_set |= BuildingType::NOBILITY; return; }
	if (!(type_set & BuildingType::RELIGION)) { type_set |= BuildingType::RELIGION; return; }
	if (!(type_set & BuildingType::SPECIAL)) { type_set |= BuildingType::SPECIAL; return; }
}

void HauntedCity::generateObserver(int bel) {
	std::function<void(int, int&)>func = std::bind(hauntedCityObserverFunc,
		bel, std::placeholders::_1, std::placeholders::_2);
	hauntedCityObserver.observe(func);
}

void HauntedCity::stopObserver() {
	hauntedCityObserver.stop();
}