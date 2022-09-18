#pragma once

#include"character.h"

class Merchant :public Character {
public:
	Merchant();
	void activateBuild(int&) override;
private:
};