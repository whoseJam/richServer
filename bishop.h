#pragma once

#include"character.h"

class Bishop :public Character {
public:
	Bishop();
	void sendCardToChoose(int) override;
	void activateBuild(int&) override;
private:
};