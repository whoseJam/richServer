#pragma once

#include"building.h"

class GraveYard :public Building {
public:
	GraveYard();
	void generateObserver(int bel) override;
	void stopObserver() override;
private:
};