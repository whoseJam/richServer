
#include<iostream>

#include"tool.h"
#include"thief.h"
#include"effect.h"
#include"gameflow.h"
#include"communicate.h"
#include"autofactory.h"

autoFactory<Character>::registerClass<Thief> regForThief("thief");

extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern Character* findCharacter(int idx);
extern int active_player;
extern int skill_regret;

struct thieveEffect :public Effect {
	void operator ()() {
		Player* my = findPlayer(launch_player);
		Player* target = findPlayer(target_player);
		int coin = target->getMoney();
		sendPublicMessage("Player" + intToString(target_player) + "被偷走了" + intToString(coin) + "枚金币");
		my->addMoney(coin);
		target->setMoney(0);
	}
};

void thieveAMan() {
	skill_regret = false;
	std::vector<std::string> choose;
	for (int rk = 3; rk <= 8; rk++) {
		std::string tmp = findCharacter(rk)->getEName() + "(character)";
		choose.push_back(tmp);
	}
	sendPublicMessage("小偷开始偷窃了");
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择被偷窃的人", choose);
	int c = receiveInt(active_player) + 2;
	Player* p = findPlayerWithCharacter(c);
	if (p != nullptr) {
		p->setIsThieved(true);
		thieveEffect* e = new thieveEffect;
		e->launch_player = active_player;
		e->target_player = p->idx;
		e->stage = EffectStage::BEFORE_COLLECT;
		addEffect(e);
	}
	sendPublicMessage("小偷行窃结束");
}

Thief::Thief() {
	setName("盗贼");
	setEName("thief");
	setMaxBuildingTime(1);
	setRank(2);

	Skill s(thieveAMan, LaunchStage::ACTION);
	s.setName("偷窃");
	pushSkill(s);
}