
#include<iostream>

#include"communicate.h"
#include"character.h"
#include"observer.h"
#include"player.h"
#include"tool.h"

extern int active_player;
extern int tot_player;
extern int skill_regret;
extern Player* findPlayer(int idx);
extern Observer<std::function<void(int, bool&)>> libraryObserver;
extern Observer<std::function<void(int, bool&)>> observatoryObserver;
extern Observer<std::function<void(int, Building*, bool&)>>administratorObserver;

int Character::getRank() {
	return rank;
}

const std::vector<Skill>& Character::getSkills() {
	return skills;
}

const std::string& Character::getName() {
	return name;
}

const std::string& Character::getEName() {
	return ename;
}

void Character::output() {
	std::cout << "<" << name << "> : ";
}

void Character::setName(std::string&& str) {
	name = std::move(str);
}

void Character::setEName(std::string&& str) {
	ename = std::move(str);
}

void Character::setMaxBuildingTime(int x) {
	maxBuildingTime = x;
}

void Character::setRank(int x) {
	rank = x;
}

void Character::pushSkill(const Skill& a) {
	skills.push_back(a);
}

void Character::popSkill() {
	skills.pop_back();
}

void Character::clear() {
	for (auto& it : skills) {
		it.setUsed(false);
	}
}

void Character::collect() {
	//Send information
	std::vector<std::string> choose = { "获得金币","抽牌" };
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "采集资源中...请选择", choose);
	sendPublicMessage("Player" + intToString(active_player) + "正在选择采集什么物资...");
	int c = receiveInt(active_player);
	if (c == 1) {
		sendPublicMessage("Player" + intToString(active_player) + "选择获得两枚金币");
		Player* p = findPlayer(active_player);
		p->addMoney(2);
	}
	else {
		sendPublicMessage("Player" + intToString(active_player) + "选择获取抽牌");
		Player* p = findPlayer(active_player);
		bool needGetCard = true;
		if (needGetCard)observatoryObserver.trigger<int, bool&>(active_player, needGetCard);
		if (needGetCard)libraryObserver.trigger<int, bool&>(active_player, needGetCard);
		if (needGetCard)p->getCardFromHeap(2, 1);
	}
	sendPrivateMessage(active_player, "风平浪静...");
}

void Character::sendMainChoice(int skill_num, int build_num) {
	std::vector<std::string> choose;
	if (skill_num > 0)choose.push_back("技能");
	else choose.push_back("技能(disabled)");
	if (build_num < maxBuildingTime)choose.push_back("建造");
	else choose.push_back("建造(disabled)");
	choose.push_back("跳过");
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "你有三个选择...", choose);
}

void Character::sendCardToChoose(int money) {
	std::vector<std::string> choose = { "我反悔了" };
	Player* p = findPlayer(active_player);
	int tot_card = p->getCardNum();
	for (int i = 0; i < tot_card; i++) {
		Building* c = p->getCard(i);
		std::string choice;
		if (c->getCost() > money) choice = c->getName() + "(building)(disabled)";
		else if (p->findBuilding(c->getName()))choice = c->getName() + "(building)(disabled)";
		else choice = c->getName() + "(building)";
		choose.push_back(choice);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "选择你的建筑...", choose);
}

void Character::activateSkill() {
	std::vector<std::string> choose;
	choose.push_back("我反悔了");
	Player* my = findPlayer(active_player);
	std::vector<Skill*> skillQueue;
	for (auto& it : skills)
		if (it.getLaunchStage() == LaunchStage::ACTION) {
			if (it.isUsed())choose.push_back(it.getName() + "(disabled)");
			else choose.push_back(it.getName());
			skillQueue.push_back(&it);
		}
	for (auto& it : my->getBuildingSkill()) {
		if (it.getLaunchStage() == LaunchStage::ACTION) {
			if (it.isUsed())choose.push_back(it.getName() + "(disabled)");
			else choose.push_back(it.getName());
			skillQueue.push_back(&it);
		}
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "选择你的技能...", choose);
	int sidx = receiveInt(active_player);
	if (sidx == 1)return;
	sidx -= 2;
	(*(skillQueue[sidx]))();
	if (!skill_regret)
		skillQueue[sidx]->setUsed(true);
}

void Character::activateBuild(int& build_num) {
	sendCardToChoose(findPlayer(active_player)->getMoney());
	int cidx = receiveInt(active_player);
	if (cidx == 1)return;
	cidx -= 2;
	Player* my = findPlayer(active_player);
	Building* card = my->takeOutCard(cidx);
	bool is_captured = false;
	administratorObserver.trigger<int, Building*, bool&>(active_player, card, is_captured);
	if (!is_captured) {
		build_num++;
		my->addMoney(-card->getCost());
		my->addBuilding(card);
	}
}

void Character::action() {
	int build_num = 0;
	while (true) {
		int skill_num =
			getSpecificSkillNum(LaunchStage::ACTION) +
			(findPlayer(active_player)->getBuildingSkill().size());
		sendMainChoice(skill_num, build_num);
		int tidx = receiveInt(active_player);
		if (tidx == 1)activateSkill();
		if (tidx == 2)activateBuild(build_num);
		if (tidx == 3)break;
	}
}

int Character::getSpecificSkillNum(LaunchStage stg) {
	int ans = 0;
	for (auto& it : skills) {
		if (it.getLaunchStage() == stg)ans++;
	}return ans;
}

void Character::launchSkillWhenCollecting() {
	for (auto& it : skills) {
		if (it.getLaunchStage() == LaunchStage::COLLECT) {
			it();
		}
	}
}

void Character::launchSkillAfterCollecting() {
	for (auto& it : skills) {
		if (it.getLaunchStage() == LaunchStage::AFTER_COLLECT) {
			it();
		}
	}
}

void Character::launchSkillBeforeAction() {
	for (auto& it : skills) {
		if (it.getLaunchStage() == LaunchStage::BEFORE_ACTION) {
			it();
		}
	}
}

void Character::launchSkillAfterAction() {
	for (auto& it : skills) {
		if (it.getLaunchStage() == LaunchStage::AFTER_ACTION) {
			it();
		}
	}
}