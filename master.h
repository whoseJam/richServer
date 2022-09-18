#pragma once

#include"character.h"

class Master :public Character {
public:
	Master();
	void sendCardToChoose(int) override;
private:
};