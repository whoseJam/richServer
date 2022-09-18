
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"effect.h"
#include"player.h"
#include"noble.h"

extern Player* findPlayer(int);
extern void getCrown();
extern int skill_regret;
extern int active_player;

autoFactory<Character>::registerClass<Noble> regForNoble("noble");

struct getCardFromNobilityEffect :public Effect {
	void operator()() {
		Player* my = findPlayer(target_player);
		int nsize = my->getSpecificBuildingNumsForMoney(BuildingType::NOBILITY);
		my->getCardFromHeap(nsize, nsize);
	}
};

void getCardFromNobility() {
	skill_regret = false;
	getCardFromNobilityEffect* e = new getCardFromNobilityEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Noble::Noble() {
	setName("¹ó×å");
	setEName("noble");
	setMaxBuildingTime(1);
	setRank(4);

	Skill s1(getCardFromNobility, LaunchStage::ACTION);
	s1.setName("Õ÷¿¨");
	Skill s2(getCrown, LaunchStage::COLLECT);
	s2.setName("¼ÓÃá");
	pushSkill(s1);
	pushSkill(s2);
}