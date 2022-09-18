#pragma once

#include"building.h"

class University :public Building {
public:
	University();
	int getValue() override;
private:
};