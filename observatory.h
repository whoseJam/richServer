#pragma once


#include"building.h"

class Observatory :public Building {
public:
	Observatory();
	void generateObserver(int) override;
	void stopObserver() override;
private:
};