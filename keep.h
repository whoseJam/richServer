#pragma once

#include"building.h"

class Keep :public Building {
public:
	Keep();
	void generateObserver(int) override;
	void stopObserver() override;
private:
};