#pragma once

#include"building.h"

class Laboratory :public Building {
public:
	Laboratory();
	void generateObserver(int) override;
	void stopObserver() override;
private:
};