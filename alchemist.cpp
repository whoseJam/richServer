
#include"autofactory.h"
#include"communicate.h"
#include"alchemist.h"
#include"gameflow.h"
#include"observer.h"
#include"player.h"
#include"effect.h"

extern Player* findPlayer(int);
extern int active_player;
extern int skill_regret;
extern Observer<std::function<void(int, Building*, bool&)>>administratorObserver;

int Alchemist::buildingCoin = 0;

autoFactory<Character>::registerClass<Alchemist> regForAlchemist("alchemist");

struct recreateCoinEffect : public Effect {
	int coin;
	void operator()() {
		Player* tar = findPlayer(active_player);
		tar->addMoney(coin);
	}
};

void recreateCoin() {
	skill_regret = false;
	recreateCoinEffect* e = new recreateCoinEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->stage = EffectStage::INSTANT;
	e->coin = Alchemist::getBuildingCoin();
	addEffect(e);
}

int Alchemist::getBuildingCoin() {
	return buildingCoin;
}

void Alchemist::activateBuild(int& build_num) {
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

Alchemist::Alchemist() {
	setEName("alchemist");
	setMaxBuildingTime(1);
	setRank(6);

	Skill s(recreateCoin, LaunchStage::AFTER_ACTION);
	s.setName("жидь");
	pushSkill(s);
}

