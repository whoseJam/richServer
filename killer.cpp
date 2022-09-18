
#include<iostream>

#include"effect.h"
#include"player.h"
#include"killer.h"
#include"gameflow.h"
#include"character.h"
#include"communicate.h"
#include"autofactory.h"

autoFactory<Character>::registerClass<Killer> regForKiller("killer");

extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern Character* findCharacter(int idx);
extern int active_player;
extern int skill_regret;

struct killerEffect :public Effect {
	void operator ()() {
		Player* p = findPlayer(target_player);
		p->setSkipAction(true);
		p->setSkipCollect(true);
	}
};

void killAMan() {
	skill_regret = false;
	std::vector<std::string> choose;
	for (int rk = 2; rk <= 8; rk++) {
		std::string tmp = findCharacter(rk)->getEName() + "(character)";
		choose.push_back(tmp);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择杀害对象", choose);
	int c = receiveInt(active_player) + 1;
	sendPublicMessage(findCharacter(c)->getName() + "被杀害了，但是真的有这个人吗？");
	Player* p = findPlayerWithCharacter(c);
	if (p != nullptr) {
		p->setIsKilled(true);
		killerEffect* e = new killerEffect;
		e->launch_player = active_player;
		e->target_player = p->idx;
		e->stage = EffectStage::INSTANT;
		addEffect(e);
	}
}

Killer::Killer() {
	setName("杀手");
	setEName("killer");
	setMaxBuildingTime(1);
	setRank(1);

	Skill s(killAMan, LaunchStage::ACTION);
	s.setName("暗杀");
	pushSkill(s);
}