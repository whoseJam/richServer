#pragma once

#include"building.h"

class Library :public Building {
public:
	Library();
	void generateObserver(int) override;
	void stopObserver() override;
private:
};