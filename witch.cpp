

#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"player.h"
#include"effect.h"
#include"witch.h"

extern Player* findPlayerWithCharacter(int);
extern Player* findPlayer(int);
extern Character* findCharacter(int);
extern int skill_regret;
extern int active_player;

autoFactory<Character>::registerClass<Witch> regForWitch("witch");

struct curseEffect :public Effect {
	void operator()() {
		active_player = launch_player;
	}
};

void curseAMan() {
	skill_regret = false;
	std::vector<std::string> choose;
	for (int rk = 2; rk <= 8; rk++) {
		std::string tmp = findCharacter(rk)->getEName() + "(character)";
		choose.push_back(tmp);
	}
	sendPublicMessage("女巫开始诅咒了");
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择被诅咒的人", choose);
	findPlayer(active_player)->setSkipAction(true);
	int c = receiveInt(active_player) + 1;
	Player* p = findPlayerWithCharacter(c);
	if (p != nullptr) {
		p->setIsCursed(true);
		curseEffect* e = new curseEffect;
		e->launch_player = active_player;
		e->target_player = p->idx;
		e->stage = EffectStage::BEFORE_ACTION;
		addEffect(e);
	}
	sendPublicMessage("女巫诅咒结束");
	sendPublicMessage(findCharacter(c)->getName() + "被诅咒了");
}

Witch::Witch() {
	setName("女巫");
	setEName("witch");
	setMaxBuildingTime(1);
	setRank(1);

	Skill s(curseAMan, LaunchStage::COLLECT);
	s.setName("诅咒");
	pushSkill(s);
}