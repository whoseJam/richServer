

#include<iostream>

#include"gameflow.h"
#include"architect.h"
#include"player.h"
#include"effect.h"
#include"communicate.h"
#include"tool.h"

extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern int active_player;
extern int skill_regret;

autoFactory<Character>::registerClass<Architect> regForArchitect("architect");

struct getTwoMoreCardEffect :public Effect {
	void operator ()() {
		Player* p = findPlayer(target_player);
		sendPublicMessage("Player" + intToString(target_player) + "多摸了两张牌");
		p->getCardFromHeap(2, 2);
	}
};

void getTwoMoreCard() {
	skill_regret = false;
	std::vector<std::string> choose = { "是","否" };
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "额外获得两张牌吗？", choose);
	int c = receiveInt(active_player);
	if (c == 1) {
		getTwoMoreCardEffect* e = new getTwoMoreCardEffect;
		e->launch_player = active_player;
		e->target_player = active_player;
		e->stage = EffectStage::INSTANT;
		addEffect(e);
	}
}

Architect::Architect() {
	setName("建筑师");
	setEName("architect");
	setMaxBuildingTime(3);
	setRank(7);

	Skill s(getTwoMoreCard, LaunchStage::COLLECT);
	s.setName("土木魂");
	pushSkill(s);
}