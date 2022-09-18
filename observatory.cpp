
#include"autofactory.h"
#include"observatory.h"
#include"communicate.h"
#include"observer.h"
#include"player.h"

#include<functional>

autoFactory<Building>::registerClass<Observatory> regForObservatory("observatory");

extern Observer<std::function<void(int, bool&)>> observatoryObserver;
extern Player* findPlayer(int);

Observatory::Observatory() :Building(5, BuildingType::SPECIAL, "observatory") {
}

void observatoryObserverFunc(int bel, int idx, bool& needGetCard) {
	if (bel != idx)return;
	Player* p = findPlayer(idx);
	std::vector<std::string> choose = { "是","否" };
	sendPrivateMessage(idx, PrivateMessageType::SINGLE_CHOICE, "是否使用观星台技能？", choose);
	int c = receiveInt(idx);
	if (c == 1) {
		p->getCardFromHeap(3, 1);
	}
}

void Observatory::generateObserver(int bel) {
	std::function<void(int, bool&)> func = std::bind(observatoryObserverFunc, bel,
		std::placeholders::_1, std::placeholders::_2);
	observatoryObserver.observe(func);
}

void Observatory::stopObserver() {
	observatoryObserver.stop();
}