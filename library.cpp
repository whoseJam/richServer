
#include<functional>

#include"autofactory.h"
#include"communicate.h"
#include"observer.h"
#include"gameflow.h"
#include"library.h"
#include"player.h"
#include"tool.h"

autoFactory<Building>::registerClass<Library> regForLibrary("library");

extern int active_player;
extern Player* findPlayer(int);
extern Observer<std::function<void(int, bool&)>> libraryObserver;

Library::Library() :Building(6, BuildingType::SPECIAL, "library") {
}

void libraryObserverFunc(int bel, int idx, bool& needGetCard) {
	if (bel != idx) return;
	std::vector<std::string> choose = { "是","否" };
	sendPrivateMessage(idx, PrivateMessageType::SINGLE_CHOICE, "是否发动图书馆技能？", choose);
	int c = receiveInt(idx);
	if (c == 1) {
		findPlayer(idx)->getCardFromHeap(2, 2);
		needGetCard = false;
	}
	else needGetCard = true;
}

void Library::generateObserver(int bel) {
	std::function<void(int, bool&)> func = std::bind(libraryObserverFunc, bel,
		std::placeholders::_1,
		std::placeholders::_2);
	libraryObserver.observe(func);
}

void Library::stopObserver() {
	libraryObserver.stop();
}