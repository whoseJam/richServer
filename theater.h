#pragma once

#include"building.h"

class Threater :public Building {
public:
	Threater();
	void generateObserver(int) override;
	void stopObserver() override;
private:
};