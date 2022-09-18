
#include<iostream>

#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"observer.h"
#include"warload.h"
#include"player.h"
#include"effect.h"
#include"tool.h"

autoFactory<Character>::registerClass<Warload> regForWarload("warload");

extern Observer<std::function<void(int, Building*, bool&)>> keepObserver;
extern Observer<std::function<void(int, Building*, int&)>> greatWallObserver;
extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern int tot_player;
extern int active_player;
extern int skill_regret;

extern Observer<std::function<void(Building*, bool&)>> warloadDestroyAnBuilding;
extern void addCardHeap(Building*);

struct destroyBuildingEffect :public Effect {
	int building_rank;
	void operator()() {
		Player* my = findPlayer(launch_player);
		Player* target = findPlayer(target_player);
		Building* b = target->getBuilding(building_rank);
		int money = b->getCost();
		greatWallObserver.trigger<int, Building*, int&>(target_player, b, money);
		money--;
		my->addMoney(-money);
		target->takeOutBuilding(building_rank);
		bool needToDelete = true;
		warloadDestroyAnBuilding.trigger<Building*, bool&>(b, needToDelete);
		if (needToDelete)
			addCardHeap(b);
	}
};

struct getMoneyFromMilitaryEffect :public Effect {
	void operator()() {
		Player* p = findPlayer(target_player);
		int nsize = p->getSpecificBuildingNumsForMoney(BuildingType::MILITARY);
		p->addMoney(nsize);
	}
};

void destroyBuilding() {
	skill_regret = false;
	while (true) {
		std::vector<std::string> choose;
		choose.push_back("我反悔了");
		for (int i = 1; i <= tot_player; i++) {
			bool flg = false;
			Player* p = findPlayer(i);
			int tot_building = p->getTotalBuildingNums();
			int my_money = findPlayer(active_player)->getMoney();
			for (int j = 0; j < tot_building; j++) {
				Building* b = p->getBuilding(j);
				int money = b->getCost();
				bool setDisabled = false;
				greatWallObserver.trigger<int, Building*, int&>(i, b, money);
				setDisabled |= (my_money + 1 < money);
				keepObserver.trigger<int, Building*, bool&>(i, b, setDisabled);
				if (!setDisabled)flg = true;
			}
			if (flg)choose.push_back("Player" + intToString(i));
			else choose.push_back("Player" + intToString(i) + "(disabled)");
		}
		sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "将要摧毁谁的建筑？", choose);
		int c = receiveInt(active_player);
		if (c == 1) {
			skill_regret = true;
			return;
		}
		c--;
		Player* p = findPlayer(c);
		int tot_building = p->getTotalBuildingNums();
		int my_money = findPlayer(active_player)->getMoney();
		choose.clear();
		choose.push_back("我反悔了");
		for (int i = 0; i < tot_building; i++) {
			Building* b = p->getBuilding(i);
			int money = b->getCost();
			bool setDisabled = false;
			greatWallObserver.trigger<int, Building*, int&>(i, b, money);
			setDisabled |= (my_money + 1 < money);
			keepObserver.trigger<int, Building*, bool&>(i, b, setDisabled);
			if (!setDisabled)choose.push_back(b->getName() + "(building)");
			else choose.push_back(b->getName() + "(building)(disabled)");
		}
		sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "将要摧毁哪座建筑？", choose);
		int bidx = receiveInt(active_player);
		if (bidx != 1) {
			bidx -= 2;
			destroyBuildingEffect* e = new destroyBuildingEffect;
			e->launch_player = active_player;
			e->target_player = c;
			e->building_rank = bidx;
			e->stage = EffectStage::INSTANT;
			addEffect(e);
			break;
		}
	}
}

void getMoneyFromMilitary() {
	skill_regret = false;
	getMoneyFromMilitaryEffect* e = new getMoneyFromMilitaryEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Warload::Warload() {
	setName("军阀");
	setEName("warload");
	setMaxBuildingTime(1);
	setRank(8);

	Skill s1(destroyBuilding, LaunchStage::ACTION);
	s1.setName("征伐");
	Skill s2(getMoneyFromMilitary, LaunchStage::ACTION);
	s2.setName("征税");
	pushSkill(s1);
	pushSkill(s2);
}