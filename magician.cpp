
#include<iostream>

#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"magician.h"
#include"player.h"
#include"effect.h"
#include"tool.h"

autoFactory<Character>::registerClass<Magician> regForMagician("magician");

extern void addCardHeap(Building*);
extern Player* findPlayerWithCharacter(int idx);
extern Player* findPlayer(int idx);
extern int active_player;
extern int tot_player;
extern int skill_regret;

struct swapCardsWithPlayerEffect :public Effect {
	void operator ()() {
		Player* my = findPlayer(launch_player);
		Player* tar = findPlayer(target_player);
		sendPublicMessage("Player" + intToString(target_player) + "的牌被换走了！");
		std::vector<Building*> my_cards;
		std::vector<Building*> tar_cards;
		int my_tot = my->getCardNum();
		int tar_tot = tar->getCardNum();
		while (my_tot--)my_cards.push_back(my->takeOutCard(0));
		while (tar_tot--)tar_cards.push_back(tar->takeOutCard(0));
		for (auto& it : my_cards) tar->addCard(it);
		for (auto& it : tar_cards)my->addCard(it);
	}
};

void swapCards() {
	sendPublicMessage("魔术师开始换牌");
	skill_regret = false;
	std::vector<std::string> choose = { "从玩家处换牌","从牌堆处换牌" };
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "从哪里换牌？", choose);
	int c = receiveInt(active_player);
	if (c == 1) {
		sendPublicMessage("魔术师选择和Player换牌");
		choose.clear();
		for (int i = 1; i <= tot_player; i++)
			choose.push_back("player" + intToString(i));
		sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "从哪个玩家处换牌？", choose);
		int p = receiveInt(active_player);
		swapCardsWithPlayerEffect* e = new swapCardsWithPlayerEffect;
		e->launch_player = active_player;
		e->target_player = p;
		e->stage = EffectStage::INSTANT;
		addEffect(e);
	}
	else {
		Player* p = findPlayer(active_player);
		sendPublicMessage("魔术师选择和牌堆换牌");
		choose.clear();
		int tot_card = p->getCardNum();
		for (int i = 0; i < tot_card; i++) {
			Building* b = p->getCard(i);
			choose.push_back(b->getName() + "(building)");
		}
		sendPrivateMessage(active_player, PrivateMessageType::MULTIPLE_CHOICE, "请选择弃牌", choose);
		std::vector<bool> type = receiveBoolVector(active_player);
		int ans = 0;
		for (int i = type.size() - 1; i >= 0; i--) {
			if (type[i]) {
				Building* b = p->takeOutCard(i);
				addCardHeap(b);
				ans++;

			}
		}
		p->getCardFromHeap(ans, ans);
	}
}

Magician::Magician() {
	setEName("magician");
	setMaxBuildingTime(1);
	setRank(3);

	Skill s(swapCards, LaunchStage::ACTION);
	s.setName("交换卡牌");
	pushSkill(s);
}