
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"effect.h"
#include"player.h"
#include"tool.h"
#include"spy.h"

extern Player* findPlayer(int);
extern int skill_regret;
extern int active_player;
extern int tot_player;

autoFactory<Character>::registerClass<Spy> regForSpy("spy");

struct monitorEffect :public Effect {
	BuildingType t;
	void operator()() {
		Player* my = findPlayer(launch_player);
		Player* tar = findPlayer(target_player);
		int correct = tar->getSpecificCardNum(t);
		int trans = std::min(correct, tar->getMoney());
		//	std::cout << "cor=" << correct << " trans=" << trans << " gMoney=" << tar->getMoney() << "\n";
		tar->addMoney(-trans);
		my->addMoney(trans);
		if (correct)my->getCardFromHeap(correct, correct);
	}
};

void monitorAMan() {
	skill_regret = false;
	std::vector<std::string> choose;
	choose.push_back("我后悔了");
	for (int i = 1; i <= tot_player; i++) {
		Player* p = findPlayer(i);
		std::string msg = "Player" + intToString(i);
		if (i == active_player)
			msg = msg + "(disable)";
		choose.push_back(msg);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "对哪一个玩家使用技能？", choose);
	int p = receiveInt(active_player);
	if (p == 1) {
		skill_regret = true;
		return;
	}
	p--;
	choose = std::vector<std::string>{ "商业","贵族","军事","宗教","特殊" };
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择一种建筑类型", choose);
	int c = receiveInt(active_player);
	BuildingType type;
	switch (c) {
	case 1:type = BuildingType::COMMERCE; break;
	case 2:type = BuildingType::NOBILITY; break;
	case 3:type = BuildingType::MILITARY; break;
	case 4:type = BuildingType::RELIGION; break;
	case 5:type = BuildingType::SPECIAL; break;
	default:break;
	}

	monitorEffect* e = new monitorEffect;
	e->launch_player = active_player;
	e->target_player = p;
	e->t = type;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Spy::Spy() {
	setName("间谍");
	setEName("spy");
	setMaxBuildingTime(1);
	setRank(2);

	Skill s(monitorAMan, LaunchStage::ACTION);
	s.setName("监视");
	pushSkill(s);
}