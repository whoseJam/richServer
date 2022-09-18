#pragma once

#include"building.h"

class WorkShop :public Building {
public:
	WorkShop();
	void generateObserver(int) override;
	void stopObserver()override;
private:
};