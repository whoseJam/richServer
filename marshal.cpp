
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"observer.h"
#include"marshal.h"
#include"effect.h"
#include"player.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int tot_player;
extern int active_player;
extern int skill_regret;
extern void getMoneyFromMilitary();

autoFactory<Character>::registerClass<Marshal> regForMarshal("marshal");

struct snatchBuildingEffect :public Effect {
	int building_rank;
	void operator()() {
		Player* my = findPlayer(launch_player);
		Player* tar = findPlayer(target_player);
		Building* b = tar->takeOutBuilding(building_rank);
		my->addBuilding(b);
		my->addMoney(-b->getCost());
	}
};

void snatchBuilding() {
	skill_regret = false;
	std::vector<std::string> choose;
	choose.push_back("我反悔了");
	for (int i = 1; i <= tot_player; i++)
		choose.push_back("Player" + intToString(i));
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "将要摧毁谁的建筑？", choose);
	int pidx = receiveInt(active_player);
	if (pidx == 1) {
		skill_regret = true;
		return;
	}
	pidx--;
	Player* my = findPlayer(active_player);
	Player* tar = findPlayer(pidx);
	int tot_building = my->getTotalBuildingNums();
	choose.clear();
	choose.push_back("我反悔了");
	for (int i = 0; i < tot_building; i++) {
		Building* b = tar->getBuilding(i);
		if (b->getCost() > 3)choose.push_back(b->getName() + "(building)(disabled)");
		else if (b->getCost() > my->getMoney())choose.push_back(b->getName() + "(building)(disabled)");
		else choose.push_back(b->getName() + "(building)");
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "将要摧毁哪座建筑？", choose);
	int bidx = receiveInt(active_player);
	if (bidx == 1) {
		skill_regret = true;
		return;
	}
	bidx -= 2;
	snatchBuildingEffect* e = new snatchBuildingEffect;
	e->launch_player = active_player;
	e->target_player = pidx;
	e->building_rank = bidx;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Marshal::Marshal() {
	setName("元帅");
	setEName("marshal");
	setMaxBuildingTime(1);
	setRank(8);

	Skill s(getMoneyFromMilitary, LaunchStage::ACTION);
	s.setName("征税");
	pushSkill(s);
}