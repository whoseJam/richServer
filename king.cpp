
#include<iostream>

#include"tool.h"
#include"king.h"
#include"player.h"
#include"effect.h"
#include"gameflow.h"
#include"communicate.h"
#include"autofactory.h"

autoFactory<Character>::registerClass<King> regForKing("king");

extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern int active_player;
extern int target_player;
extern int skill_regret;

struct getCrownEffect :public Effect {
	void operator()() {
		Player* p = findPlayer(target_player);
		::target_player = target_player;
		sendPublicMessage("Player" + intToString(target_player) + "成为了国王");
	}
};

struct getMoneyFromNobilityEffect :public Effect {
	void operator()() {
		Player* p = findPlayer(target_player);
		int nsize = p->getSpecificBuildingNumsForMoney(BuildingType::NOBILITY);
		sendPublicMessage("Player" + intToString(target_player) + "征收了" + intToString(nsize) + "枚金币");
		p->addMoney(nsize);
	}
};

void getCrown() {
	skill_regret = false;
	getCrownEffect* e = new getCrownEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

void getMoneyFromNobility() {
	skill_regret = false;
	getMoneyFromNobilityEffect* e = new getMoneyFromNobilityEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

King::King() {
	setName("国王");
	setEName("king");
	setMaxBuildingTime(1);
	setRank(4);

	Skill s1(getCrown, LaunchStage::COLLECT);
	s1.setName("加冕");
	Skill s2(getMoneyFromNobility, LaunchStage::ACTION);
	s2.setName("征税");
	pushSkill(s1);
	pushSkill(s2);
}