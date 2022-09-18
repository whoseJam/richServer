
#include<iostream>

#include"building.h"

int Building::getType() {
	return type;
}

int Building::getSymbolType() {
	return type;
}

int Building::getCost() {
	return cost;
}

int Building::getValue() {
	return cost;
}

const std::string& Building::getName() {
	return name;
}

void Building::generateObserver(int) {
}
void Building::stopObserver() {
}

void Building::setName(const std::string& str) {
	name = str;
}

void Building::setCost(int c) {
	cost = c;
}

void Building::setType(BuildingType t) {
	type = t;
}

void Building::output() {
	std::cout << "[" << name << "] ";
	std::cout << "<";
	switch (type)
	{
	case RELIGION:
		std::cout << "宗教";
		break;
	case MILITARY:
		std::cout << "军队";
		break;
	case COMMERCE:
		std::cout << "商业";
		break;
	case NOBILITY:
		std::cout << "贵族";
		break;
	case SPECIAL:
		std::cout << "特殊";
		break;
	default:
		break;
	}
	std::cout << ">";
	std::cout << " 费用:" << cost << "\n";
}