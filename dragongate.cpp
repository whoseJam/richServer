
#include"communicate.h"
#include"autofactory.h"
#include"dragongate.h"
#include"gameflow.h"
#include"effect.h"
#include"player.h"
#include"tool.h"

extern int active_player;
extern Player* findPlayer(int);

autoFactory<Building>::registerClass<DragonGate> regForDragonGate("dragongate");

DragonGate::DragonGate() :Building(6, BuildingType::SPECIAL, "dragongate") {
}

int DragonGate::getValue() {
	return 8;
}