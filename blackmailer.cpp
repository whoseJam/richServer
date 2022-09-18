
#include"autofactory.h"
#include"communicate.h"
#include"blackmailer.h"
#include"gameflow.h"
#include"effect.h"
#include"player.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int active_player;
extern int tot_player;
extern int skill_regret;

autoFactory<Character>::registerClass<Blackmailer> regForBlackmailer("blackmailer");

struct extortionEffect :public Effect {
	bool flower;
	void operator()() {
		Player* my = findPlayer(launch_player);
		Player* tar = findPlayer(target_player);
		std::vector<std::string>choose{ "是", "否" };
		sendPrivateMessage(target_player, PrivateMessageType::SINGLE_CHOICE, "是否交钱？", choose);
		int tidx = receiveInt(target_player);
		if (tidx == 1) {
			int trans = my->getMoney() / 2;
			my->addMoney(trans);
			tar->addMoney(-trans);
		}
		else if (flower) {
			int trans = my->getMoney();
			my->addMoney(trans);
			tar->addMoney(-trans);
		}
	}
};

void extortionAMan() {
	skill_regret = false;

	std::vector<std::string>choose;
	for (int i = 1; i <= tot_player; i++) {
		std::string tmp = "Player" + intToString(i);
		Player* my = findPlayer(i);
		if (my->getCharacterRank() == 1 || i == active_player)tmp = tmp + "(disabled)";
		choose.push_back(tmp);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择鲜花标记者", choose);
	int tidx = receiveInt(active_player);
	choose.clear();
	for (int i = 1; i <= tot_player; i++) {
		std::string tmp = "Player" + intToString(i);
		Player* my = findPlayer(i);
		if (my->getCharacterRank() == 1 || i == active_player || i == tidx)tmp = tmp + "(disabled)";
		choose.push_back(tmp);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择非鲜花标记者", choose);
	int ridx = receiveInt(active_player);

	extortionEffect* e1 = new extortionEffect;
	extortionEffect* e2 = new extortionEffect;
	e1->launch_player = active_player;
	e2->launch_player = active_player;
	e1->target_player = tidx;
	e2->target_player = ridx;
	e1->flower = true;
	e2->flower = false;
	e1->stage = EffectStage::BEFORE_ACTION;
	e2->stage = EffectStage::BEFORE_ACTION;
	addEffect(e1);
	addEffect(e2);
}

Blackmailer::Blackmailer() {
	setName("勒索者");
	setEName("blackmailer");
	setMaxBuildingTime(1);
	setRank(2);

	Skill s(extortionAMan, LaunchStage::ACTION);
	s.setName("勒索");
	pushSkill(s);
}
