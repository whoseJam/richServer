
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"merchant.h"
#include"observer.h"
#include"effect.h"
#include"player.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int active_player;
extern void getMoneyFromCommerce();
extern Observer<std::function<void(int, Building*, bool&)>>administratorObserver;

autoFactory<Character>::registerClass<Merchant> regForMerchant("merchant");

void Merchant::activateBuild(int& build_num) {
	sendCardToChoose(findPlayer(active_player)->getMoney());
	int cidx = receiveInt(active_player);
	if (cidx == 1)return;
	cidx -= 2;
	Player* my = findPlayer(active_player);
	Building* card = my->takeOutCard(cidx);
	bool is_captured = false;
	administratorObserver.trigger<int, Building*, bool&>(active_player, card, is_captured);
	if (!is_captured) {
		if (card->getType() != BuildingType::COMMERCE)build_num++;
		my->addMoney(-card->getCost());
		my->addBuilding(card);
	}
}

Merchant::Merchant() {
	setName("ÂòÂôÈË");
	setEName("merchant");
	setMaxBuildingTime(1);
	setRank(6);

	Skill s(getMoneyFromCommerce, LaunchStage::ACTION);
	s.setName("Õ÷Ë°");
	pushSkill(s);
}