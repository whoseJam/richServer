
#include<iostream>

#include"tool.h"
#include"abbot.h"
#include"player.h"
#include"effect.h"
#include"gameflow.h"
#include"communicate.h"

extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern int active_player;
extern int skill_regret;

autoFactory<Character>::registerClass<Abbot> regForAbbot("abbot");

struct getMoneyFromReligionEffect :public Effect {
	void operator()() {
		Player* p = findPlayer(target_player);
		int nsize = p->getSpecificBuildingNumsForMoney(BuildingType::RELIGION);
		sendPublicMessage("Player" + intToString(target_player) + "征收了" + intToString(nsize) + "枚金币");
		p->addMoney(nsize);
	}
};

void getMoneyFromReligion() {
	skill_regret = false;
	getMoneyFromReligionEffect* e = new getMoneyFromReligionEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Abbot::Abbot() {
	setName("住持");
	setEName("abbot");
	setMaxBuildingTime(1);
	setRank(5);

	Skill s(getMoneyFromReligion, LaunchStage::ACTION);
	s.setName("征税");
	pushSkill(s);
}