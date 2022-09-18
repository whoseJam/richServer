
#include"autofactory.h"
#include"communicate.h"
#include"laboratory.h"
#include"observer.h"
#include"gameflow.h"
#include"player.h"
#include"effect.h"
#include"skill.h"

#include<functional>

autoFactory<Building>::registerClass<Laboratory> regForLaboratory("laboratory");

extern Observer<std::function<void(int)>> laboratoryPushObserver;
extern Observer<std::function<void(int)>> laboratoryPopObserver;
extern Player* findPlayer(int);
extern int skill_regret;
extern int active_player;

Laboratory::Laboratory() :Building(5, BuildingType::SPECIAL, "laboratory") {
}

struct MakeCardToMoneyEffect :public Effect {
	int card_rank;
	void operator()() {
		Player* p = findPlayer(target_player);
		Building* b = p->takeOutCard(card_rank);
		p->addMoney(1);
		delete b;// ???
	}
};

void makeCardToMoney() {
	skill_regret = false;
	std::vector<std::string> choose;
	choose.push_back("我反悔了");
	Player* p = findPlayer(active_player);
	int tot_card = p->getCardNum();
	for (int i = 0; i < tot_card; i++) {
		Building* b = p->getCard(i);
		choose.push_back(b->getName() + "(building)");
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "想将哪张卡变化成一枚金币？", choose);
	int c = receiveInt(active_player);
	if (c == 1) { skill_regret = true; return; }
	c -= 2;
	MakeCardToMoneyEffect* e = new MakeCardToMoneyEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->card_rank = c;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

void laboratoryPushObserverFunc(int bel, int idx) {
	if (bel != idx)return;
	Player* p = findPlayer(idx);
	Skill s(makeCardToMoney, LaunchStage::ACTION);
	s.setName("卡->金币");
	p->pushBuildingSkill(s);
}

void laboratoryPopObserverFunc(int bel, int idx) {
	if (bel != idx)return;
	Player* p = findPlayer(idx);
	p->popBuildingSkill();
}

void Laboratory::generateObserver(int bel) {
	std::function<void(int)>pushFunc = std::bind(laboratoryPushObserverFunc, bel, std::placeholders::_1);
	std::function<void(int)> popFunc = std::bind(laboratoryPopObserverFunc, bel, std::placeholders::_1);
	laboratoryPushObserverFunc(bel, bel);
	laboratoryPushObserver.observe(pushFunc);
	laboratoryPopObserver.observe(popFunc);
}

void Laboratory::stopObserver() {
	laboratoryPushObserver.stop();
	laboratoryPopObserver.stop();
}