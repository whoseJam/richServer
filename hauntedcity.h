#pragma once

#include"building.h"

class HauntedCity :public Building {
public:
	HauntedCity();
	void generateObserver(int bel) override;
	void stopObserver() override;
	int getSymbolType() override;
private:
};