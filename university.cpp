
#include"autofactory.h"
#include"university.h"

autoFactory<Building>::registerClass<University> regForUniversity("university");

University::University() :Building(6, BuildingType::SPECIAL, "university") {
}

int University::getValue() {
	return 8;
}