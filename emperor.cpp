
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"emperor.h"
#include"player.h"
#include"effect.h"
#include"tool.h"

extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern int active_player;
extern int target_player;
extern int skill_regret;
extern int tot_player;
extern void getMoneyFromNobility();

autoFactory<Character>::registerClass<Emperor> regForEmperor("emperor");

struct coronatedEffect :public Effect {
	int ctype;
	void operator()() {
		Player* my = findPlayer(launch_player);
		Player* tar = findPlayer(target_player);
		if (ctype == 1) {
			int trans = std::min(1, tar->getMoney());
			tar->addMoney(-trans);
			my->addMoney(trans);
		}
		else {
			int tot_card = tar->getCardNum();
			if (tot_card) {
				int idx = Rand(0, tot_card - 1);
				Building* b = tar->takeOutCard(idx);
				my->addCard(b);
			}
		}
	}
};

void coronateAMan() {
	skill_regret = false;
	std::vector<std::string> choose;
	for (int i = 1; i <= tot_player; i++) {
		std::string tmp = "Player" + intToString(i);
		if (i == active_player)tmp = tmp + "(disabled)";
		choose.push_back(tmp);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择被加冕玩家", choose);
	int pidx = receiveInt(active_player);
	choose = std::vector<std::string>{ "获得金币","获得卡牌" };
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择获得物", choose);
	int tidx = receiveInt(active_player);
	coronatedEffect* e = new coronatedEffect;
	e->launch_player = active_player;
	e->target_player = pidx;
	e->ctype = tidx;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Emperor::Emperor() {
	setEName("emperor");
	setMaxBuildingTime(1);
	setRank(4);

	Skill s1(coronateAMan, LaunchStage::AFTER_COLLECT);
	s1.setName("加冕");
	Skill s2(getMoneyFromNobility, LaunchStage::ACTION);
	s2.setName("征税");
	pushSkill(s1);
	pushSkill(s2);
}