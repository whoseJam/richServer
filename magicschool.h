#pragma once

#include"building.h"

class MagicSchool :public Building {
public:
	MagicSchool();
	void generateObserver(int) override;
	void stopObserver() override;
private:
};