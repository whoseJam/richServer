#pragma once

#include"building.h"

class GreatWall :public Building {
public:
	GreatWall();
	void generateObserver(int) override;
	void stopObserver() override;
private:
};