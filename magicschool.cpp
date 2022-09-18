
#include"autofactory.h"
#include"magicschool.h"
#include"observer.h"

#include<functional>

autoFactory<Building>::registerClass<MagicSchool> regForMagicSchool("magicschool");

extern Observer<std::function<void(int, int&)>> magicSchoolObserver;

MagicSchool::MagicSchool() :Building(6, BuildingType::SPECIAL, "magicschool") {
}

void magicSchoolObserverFunc(int bel, int idx, int& nsize) {
	if (bel != idx)return;
	nsize++;
}

void MagicSchool::generateObserver(int bel) {
	std::function<void(int, int&)> func = std::bind(magicSchoolObserverFunc,
		bel, std::placeholders::_1, std::placeholders::_2);
	magicSchoolObserver.observe(func);
}

void MagicSchool::stopObserver() {
	magicSchoolObserver.stop();
}