
#include"autofactory.h"
#include"communicate.h"
#include"observer.h"
#include"gameflow.h"
#include"effect.h"
#include"player.h"
#include"bishop.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int skill_regret;
extern int active_player;
extern int tot_player;
extern Observer<std::function<void(int, Building*, bool&)>>administratorObserver;

autoFactory<Character>::registerClass<Bishop> regForBishop("bishop");

struct getCardFromReligionEffect :public Effect {
	void operator()() {
		Player* my = findPlayer(target_player);
		int nsize = my->getSpecificBuildingNumsForMoney(BuildingType::RELIGION);
		my->getCardFromHeap(nsize, nsize);
	}
};

void getCardFromReligion() {
	skill_regret = false;
	getCardFromReligionEffect* e = new getCardFromReligionEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Bishop::Bishop() {
	setName("主教");
	setEName("bishop");
	setMaxBuildingTime(1);
	setRank(5);

	Skill s1(getCardFromReligion, LaunchStage::ACTION);
	s1.setName("征卡");
	pushSkill(s1);
}

void Bishop::sendCardToChoose(int money) {
	std::vector<std::string> choose = { "我反悔了" };
	Player* p = findPlayer(active_player);
	int tot_card = p->getCardNum();
	for (int i = 0; i < tot_card; i++) {
		Building* c = p->getCard(i);
		std::string choice;
		if (p->findBuilding(c->getName()))choice = c->getName() + "(building)(disabled)";
		else if (c->getCost() > money + p->getCardNum())choice = c->getName() + "(building)(disabled)";
		else choice = c->getName() + "(building)";
		choose.push_back(choice);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "选择你的建筑...", choose);
}

void Bishop::activateBuild(int& build_num) {
	sendCardToChoose(findPlayer(active_player)->getMoney());
	int cidx = receiveInt(active_player);
	if (cidx == 1)return;
	cidx -= 2;
	Player* my = findPlayer(active_player);
	Building* card = my->takeOutCard(cidx);
	if (card->getCost() <= my->getMoney()) {
		bool is_captured = false;
		administratorObserver.trigger<int, Building*, bool&>(active_player, card, is_captured);
		if (!is_captured) {
			build_num++;
			my->addMoney(-card->getCost());
			my->addBuilding(card);
		}
	}
	else {
		int del = card->getCost() - my->getMoney();
		std::vector<std::string> choose{ "我后悔了" };
		for (int i = 1; i <= tot_player; i++) {
			std::string tmp = "Player" + intToString(i);
			if (active_player == i || findPlayer(i)->getMoney() < del)tmp = tmp + "(disabled)";
			choose.push_back(tmp);
		}
		sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "选择一个玩家来凑钱", choose);
		int pidx = receiveInt(active_player);
		if (pidx == 1)return;
		pidx--;
		Player* tar = findPlayer(pidx);
		choose.clear();
		int tot_card = my->getCardNum();
		for (int i = 0; i < tot_card; i++) {
			Building* c = my->getCard(i);
			choose.push_back(c->getName() + "(building)");
		}
		sendConstPrivateMessage(active_player, del, "请给出一定卡牌", choose);
		std::vector<bool> result = receiveBoolVector(active_player);
		for (int i = tot_card - 1; i >= 0; i--) {
			if (result[i]) {
				Building* c = my->takeOutCard(i);
				tar->addCard(c);
			}
		}

		bool is_captured = false;
		administratorObserver.trigger<int, Building*, bool&>(active_player, card, is_captured);
		if (!is_captured) {
			build_num++;
			my->setMoney(0);
			my->addBuilding(card);
		}
	}
}