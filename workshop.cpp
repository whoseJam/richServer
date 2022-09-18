
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"workshop.h"
#include"observer.h"
#include"player.h"
#include"effect.h"

#include<functional>

autoFactory<Building>::registerClass<WorkShop> regForWorkShop("workshop");

extern Observer<std::function<void(int)>> workshopPushObserver;
extern Observer<std::function<void(int)>> workshopPopObserver;
extern Player* findPlayer(int);
extern int skill_regret;
extern int active_player;

WorkShop::WorkShop() :Building(5, BuildingType::SPECIAL, "workshop") {
}

struct MakeMoneyToCardEffect :public Effect {
	void operator ()() {
		Player* p = findPlayer(target_player);
		p->addMoney(-3);
		p->getCardFromHeap(2, 2);
	}
};

void makeMoneyToCard() {
	skill_regret = false;
	std::vector<std::string> choose;
	choose.push_back("我反悔了");
	Player* p = findPlayer(active_player);
	if (p->getMoney() >= 3)choose.push_back("确认");
	else choose.push_back("确认(disabled)");
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "确定用三枚金币换取两张卡牌吗？", choose);
	int c = receiveInt(active_player);
	if (c == 1) { skill_regret = true; return; }
	MakeMoneyToCardEffect* e = new MakeMoneyToCardEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

void workshopPushObserverFunc(int bel, int idx) {
	if (bel != idx)return;
	Skill s(makeMoneyToCard, LaunchStage::ACTION);
	s.setName("金币->卡");
	Player* p = findPlayer(idx);
	p->pushBuildingSkill(s);
}

void workshopPopObserverFunc(int bel, int idx) {
	if (bel != idx)return;
	findPlayer(idx)->popBuildingSkill();
}

void WorkShop::generateObserver(int bel) {
	std::function<void(int)> pushFunc = std::bind(workshopPushObserverFunc, bel, std::placeholders::_1);
	std::function<void(int)> popFunc = std::bind(workshopPopObserverFunc, bel, std::placeholders::_1);
	workshopPushObserverFunc(bel, bel);
	workshopPushObserver.observe(pushFunc);
	workshopPopObserver.observe(popFunc);
}

void WorkShop::stopObserver() {
	workshopPushObserver.stop();
	workshopPopObserver.stop();
}