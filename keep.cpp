
#include"autofactory.h"
#include"observer.h"
#include"keep.h"

#include<functional>

autoFactory<Building>::registerClass<Keep> regForKeep("keep");

extern Observer<std::function<void(int, Building*, bool&)>> keepObserver;

Keep::Keep() :Building(3, BuildingType::SPECIAL, "keep") {
}

void keepObserverFunc(int bel, int idx, Building* b, bool& setDisabled) {
	if (bel != idx)return;
	if (b->getName() == "watchtower")setDisabled = true;
	else setDisabled = false;
}

void Keep::generateObserver(int bel) {
	std::function<void(int, Building*, bool&)> func = std::bind(
		keepObserverFunc, bel,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3
	);
	keepObserver.observe(func);
}

void Keep::stopObserver() {
	keepObserver.stop();
}