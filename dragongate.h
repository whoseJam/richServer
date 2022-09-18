#pragma once

#include"building.h"

class DragonGate :public Building {
public:
	DragonGate();
	int getValue() override;
private:
};