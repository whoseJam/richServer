
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"master.h"
#include"player.h"
#include"effect.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int tot_player;
extern int skill_regret;
extern int active_player;

autoFactory<Character>::registerClass<Master> regForMaster("master");

struct thieveCardEffect :public Effect {
	bool build_instant;
	int card_rank;
	void operator()() {
		Player* my = findPlayer(launch_player);
		Player* tar = findPlayer(target_player);
		Building* b = tar->takeOutCard(card_rank);
		if (build_instant) {
			my->addMoney(-(b->getCost()));
			my->addBuilding(b);
		}
		else my->addCard(b);
	}
};

void thieveACard() {
	skill_regret = false;
	std::vector<std::string> choose;
	choose.push_back("�ҷ�����");
	for (int i = 1; i <= tot_player; i++) {
		std::string tmp = "Player" + intToString(i);
		if (i == active_player || findPlayer(i)->getCardNum() == 0)
			tmp = tmp + "(disabled)";
		choose.push_back(tmp);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "��ѡ��һ�����", choose);
	int pidx = receiveInt(active_player);
	if (pidx == 1) {
		skill_regret = true;
		return;
	}
	pidx--;
	Player* p = findPlayer(pidx);
	choose.clear();
	int tot_card = p->getCardNum();
	for (int i = 0; i < tot_card; i++) {
		Building* b = p->getCard(i);
		std::string tmp = b->getName() + "(building)";
		choose.push_back(tmp);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "��ѡ��һ������", choose);
	int bidx = receiveInt(active_player) - 1;
	choose.clear();
	if (p->getCard(bidx)->getCost() <= findPlayer(active_player)->getMoney()) choose.push_back("��");
	else choose.push_back("��(disabled)");
	choose.push_back("��");
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "�Ƿ����̽��죿", choose);
	int type = receiveInt(active_player);

	thieveCardEffect* e = new thieveCardEffect;
	e->launch_player = active_player;
	e->target_player = pidx;
	e->build_instant = (type == 1);
	e->stage = EffectStage::INSTANT;
	e->card_rank = bidx;
	addEffect(e);
}

void Master::sendCardToChoose(int money) {
	std::vector<std::string> choose = { "�ҷ�����" };
	Player* p = findPlayer(active_player);
	int tot_card = p->getCardNum();
	for (int i = 0; i < tot_card; i++) {
		Building* c = p->getCard(i);
		std::string choice;
		if (c->getCost() > money) choice = c->getName() + "(building)(disabled)";
		else choice = c->getName() + "(building)";
		choose.push_back(choice);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "ѡ����Ľ���...", choose);
}

Master::Master() {
	setName("��ʦ");
	setEName("master");
	setMaxBuildingTime(1);
	setRank(3);

	Skill s(thieveACard, LaunchStage::ACTION);
	s.setName("ʩ��");
	pushSkill(s);
}