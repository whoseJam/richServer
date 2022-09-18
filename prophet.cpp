
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"prophet.h"
#include"effect.h"
#include"player.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int active_player;
extern int tot_player;
extern int skill_regret;

autoFactory<Character>::registerClass<Prophet> regForProphet("prophet");

void collectCards() {
	skill_regret = false;
	Player* my = findPlayer(active_player);
	std::vector<std::string> choose;
	std::vector<int>pidx;
	for (int i = 1; i <= tot_player; i++) {
		if (i != active_player) {
			Player* tar = findPlayer(i);
			if (tar->getCardNum() == 0)continue;
			Building* card = tar->takeOutRandomCard();
			my->addCard(card);
			pidx.push_back(i);
		}
	}
	for (auto& it : pidx) {
		choose.clear();
		int tot_card = my->getCardNum();
		for (int i = 0; i < tot_card; i++) {
			choose.push_back(my->getCard(i)->getName() + "(building)");
		}
		sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "请为Player" + intToString(it) + "选择一张卡牌", choose);
		int cidx = receiveInt(active_player); cidx--;
		Building* card = my->takeOutCard(cidx);
		Player* tar = findPlayer(it);
		tar->addCard(card);
	}
}

Prophet::Prophet() {
	setName("预言家");
	setEName("prophet");
	setMaxBuildingTime(2);
	setRank(3);

	Skill s(collectCards, LaunchStage::ACTION);
	s.setName("交换资源");
	pushSkill(s);
}