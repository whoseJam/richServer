
#include<iostream>

#include"tool.h"
#include"trader.h"
#include"player.h"
#include"effect.h"
#include"gameflow.h"
#include"communicate.h"
#include"autofactory.h"

autoFactory<Character>::registerClass<Trader> regForTrader("trader");

extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern int active_player;
extern int skill_regret;

struct getOneMoreCoinEffect :public Effect {
	void operator ()() {
		Player* p = findPlayer(target_player);
		sendPublicMessage("Player" + intToString(target_player) + "额外获得了一枚金币");
		p->addMoney(1);
	}
};

struct getMoneyFromCommerceEffect :public Effect {
	void operator ()() {
		Player* p = findPlayer(target_player);
		int nsize = p->getSpecificBuildingNumsForMoney(BuildingType::COMMERCE);
		sendPublicMessage("Player" + intToString(target_player) + "征收了" + intToString(nsize) + "枚金币");
		p->addMoney(nsize);
	}
};

void getOneMoreMoney() {
	skill_regret = false;
	std::vector<std::string> choose = { "是","否" };
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "额外获得一枚金币吗？", choose);
	int c = receiveInt(active_player);
	if (c == 1) {
		getOneMoreCoinEffect* e = new getOneMoreCoinEffect;
		e->launch_player = active_player;
		e->target_player = active_player;
		e->stage = EffectStage::INSTANT;
		addEffect(e);
	}
}

void getMoneyFromCommerce() {
	skill_regret = false;
	getMoneyFromCommerceEffect* e = new getMoneyFromCommerceEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Trader::Trader() {
	setName("商人");
	setEName("trader");
	setMaxBuildingTime(1);
	setRank(6);

	Skill s1(getOneMoreMoney, LaunchStage::COLLECT);
	s1.setName("+1金币");
	Skill s2(getMoneyFromCommerce, LaunchStage::ACTION);
	s2.setName("征税");
	pushSkill(s1);
	pushSkill(s2);
}