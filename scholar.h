#pragma once

#include"character.h"

class Scholar :public Character {
public:
	Scholar();
	void collect() override;
private:
};