
#include"communicate.h"
#include"autofactory.h"
#include"graveyard.h"
#include"observer.h"
#include"player.h"

extern Player* findPlayer(int);
extern Observer<std::function<void(Building*, bool&)>> warloadDestroyAnBuilding;

autoFactory<Building>::registerClass<GraveYard> regForGraveYard("graveyard");

GraveYard::GraveYard() :Building(5, BuildingType::SPECIAL, "graveyard") {
}

void getDestroyedBuilding(Building* b, bool& needToDelete, int bel) {
	if (findPlayer(bel)->getCharacterRank() == 8)return;
	std::vector<std::string> choose;
	Player* p = findPlayer(bel);
	if (p->getMoney() >= 1)choose.push_back("是");
	else choose.push_back("是(disabled)");
	choose.push_back("否");
	sendPrivateMessage(bel, PrivateMessageType::SINGLE_CHOICE, "是否使用一枚金币换取被拆毁的" + b->getName(), choose);
	int c = receiveInt(bel);
	if (c == 1) {
		p->addCard(b);
		p->addMoney(-1);
		needToDelete = false;
	}
	else needToDelete = true;
}

void GraveYard::generateObserver(int bel) {
	std::function<void(Building*, bool&)> func = std::bind(
		getDestroyedBuilding,
		std::placeholders::_1,
		std::placeholders::_2, bel);
	warloadDestroyAnBuilding.observe(func);
}

void GraveYard::stopObserver() {
	warloadDestroyAnBuilding.stop();
}