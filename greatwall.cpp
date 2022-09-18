
#include"autofactory.h"
#include"greatwall.h"
#include"observer.h"

#include<functional>

autoFactory<Building>::registerClass<GreatWall> regForGreatWall("greatwall");

extern Observer<std::function<void(int, Building*, int&)>> greatWallObserver;


GreatWall::GreatWall() :Building(6, BuildingType::SPECIAL, "greatwall") {
}

void greatWallObserverFunc(int bel, int idx, Building* b, int& money) {
	if (bel != idx)return;
	if (b->getName() != "greatwall")
		money++;
}

void GreatWall::generateObserver(int bel) {
	std::function<void(int, Building*, int&)>func = std::bind(
		greatWallObserverFunc, bel,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3
	);
	greatWallObserver.observe(func);
}

void GreatWall::stopObserver() {
	greatWallObserver.stop();
}