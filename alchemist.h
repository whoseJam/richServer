#pragma once

#include"character.h"

class Alchemist :public Character {
public:
	Alchemist();
	void activateBuild(int&) override;
	static int getBuildingCoin();
private:
	static int buildingCoin;
};