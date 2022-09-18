
#include"autofactory.h"
#include"communicate.h"
#include"diplomat.h"
#include"gameflow.h"
#include"player.h"
#include"effect.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int active_player;
extern int skill_regret;
extern int tot_player;
extern void getMoneyFromMilitary();

autoFactory<Character>::registerClass<Diplomat> regForDiplomat("diplomat");

struct swapBuildingEffect : public Effect {
	int my_bidx;
	int tar_bidx;
	void operator()() {
		Player* my = findPlayer(launch_player);
		Player* tar = findPlayer(target_player);
		Building* my_building = my->takeOutBuilding(my_bidx);
		Building* tar_building = my->takeOutBuilding(tar_bidx);
		int delta = std::max(0, tar_building->getCost() - my_building->getCost());
		if (delta)my->addMoney(-delta);
		my->addBuilding(tar_building);
		tar->addBuilding(my_building);
	}
};

void exchangeBuilding() {
	skill_regret = false;
	while (true) {
		std::vector<std::string>choose;
		Player* my = findPlayer(active_player);
		int my_tot_building = my->getTotalBuildingNums();
		choose.push_back("我后悔了");
		for (int i = 0; i < my_tot_building; i++) {
			choose.push_back(my->getBuilding(i)->getName() + "(building)");
		}
		sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择自己交换出去的建筑", choose);
		int my_bidx = receiveInt(active_player);
		if (my_bidx == 1) {
			skill_regret = true;
			return;
		}
		my_bidx -= 2;
		Building* my_building = my->getBuilding(my_bidx);

		choose = std::vector<std::string>{ "我后悔了" };
		for (int i = 1; i <= tot_player; i++) {
			if (i == active_player) continue;
			Player* tar = findPlayer(i);
			int tar_tot_building = tar->getTotalBuildingNums();
			int flag = false;
			for (int j = 0; j < tar_tot_building; j++) {
				Building* tar_building = tar->getBuilding(j);
				if (tar_building->getCost() <= my_building->getCost() + my->getMoney()) {
					flag = true;
				}
			}
			if (flag)choose.push_back("Player" + intToString(i));
			else choose.push_back("Player" + intToString(i) + "(disabled)");
		}

		sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择谈判玩家", choose);
		int pidx = receiveInt(active_player);
		if (pidx == 1) {
			skill_regret = true;
			return;
		}
		pidx -= 1;

		Player* tar = findPlayer(pidx);
		int tar_tot_building = tar->getTotalBuildingNums();
		choose = std::vector<std::string>{ "我后悔了" };
		for (int i = 0; i < tar_tot_building; i++) {
			Building* tar_building = tar->getBuilding(i);
			if (tar_building->getCost() <= my_building->getCost() + my->getMoney())
				choose.push_back(tar_building->getName()+"(building)");
			else choose.push_back(tar_building->getName() + "(building)(disabled)");
		}
		sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请选择对方建筑", choose);
		int tar_bidx = receiveInt(active_player);
		if (tar_bidx == 1) {
			skill_regret = true;
			return;
		}

		swapBuildingEffect* e = new swapBuildingEffect;
		e->launch_player = active_player;
		e->target_player = pidx;
		e->my_bidx = my_bidx;
		e->tar_bidx = tar_bidx;
		e->stage = EffectStage::INSTANT;
		addEffect(e);
	}
}

Diplomat::Diplomat() {
	setEName("diplomat");
	setMaxBuildingTime(1);
	setRank(8);

	Skill s1(exchangeBuilding, LaunchStage::ACTION);
	s1.setName("谈判");
	Skill s2(getMoneyFromMilitary,LaunchStage::ACTION);
	s2.setName("征税");
	pushSkill(s1);
	pushSkill(s2);
}