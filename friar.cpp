
#include<iostream>

#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"effect.h"
#include"player.h"
#include"friar.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int skill_regret;
extern int active_player;
extern int tot_player;

autoFactory<Character>::registerClass<Friar> regForFriar("friar");

struct getResourceFromReligionEffect :public Effect {
	int nCard;
	int nMoney;
	void operator()() {
		Player* tar = findPlayer(target_player);
		tar->addMoney(nMoney);
		tar->getCardFromHeap(nCard, nCard);
	}
};

struct begForMoneyEffect :public Effect {
	void operator()() {
		Player* tar = findPlayer(target_player);
		Player* my = findPlayer(launch_player);
		tar->addMoney(-1);
		my->addMoney(1);
	}
};

void getResourceFromReligion() {
	skill_regret = false;
	std::vector<std::string> choose;
	Player* p = findPlayer(active_player);
	int nsize = p->getSpecificBuildingNumsForMoney(BuildingType::RELIGION);
	for (int ncard = 0; ncard <= nsize; ncard++) {
		std::string tmp = "卡牌:" + intToString(ncard) + "+金币:" + intToString(nsize - ncard);
		choose.push_back(tmp);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择方案", choose);
	int type = receiveInt(active_player);
	getResourceFromReligionEffect* e = new getResourceFromReligionEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->nCard = type - 1;
	e->nMoney = nsize - (e->nCard);
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

void begForMoney() {
	skill_regret = false;
	int mxMoney = 0;
	for (int i = 1; i <= tot_player; i++) {
		if (i == active_player)continue;
		mxMoney = std::max(findPlayer(i)->getMoney(), mxMoney);
	}
	std::vector<std::string> choose;
	choose.push_back("我后悔了");
	int my_min_coin = findPlayer(active_player)->getMinMoneyInARound();
	std::cout << "my_min_coin=" << my_min_coin << "\n";
	for (int i = 1; i <= tot_player; i++) {
		if (my_min_coin < mxMoney && findPlayer(i)->getMoney() == mxMoney && i != active_player)
			choose.push_back("Player" + intToString(i));
		else choose.push_back("Player" + intToString(i) + "(disabled)");
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择从最富有的玩家处乞讨", choose);
	int pidx = receiveInt(active_player);
	if (pidx == 1) {
		skill_regret=true;
		return;
	}
	pidx-=1;
	begForMoneyEffect* e = new begForMoneyEffect;
	e->launch_player = active_player;
	e->target_player = pidx;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Friar::Friar() {
	setEName("friar");
	setMaxBuildingTime(1);
	setRank(5);

	Skill s1(getResourceFromReligion, LaunchStage::ACTION);
	s1.setName("赎罪券");
	Skill s2(begForMoney, LaunchStage::ACTION);
	s2.setName("乞讨");
	pushSkill(s1);
	pushSkill(s2);
}