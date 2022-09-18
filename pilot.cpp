
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"player.h"
#include"effect.h"
#include"pilot.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int active_player;
extern int skill_regret;

autoFactory<Character>::registerClass<Pilot> regForPilot("pilot");

struct getMoreResourceEffect :public Effect {
	int resource_type;
	void operator()() {
		Player* tar = findPlayer(target_player);
		if (resource_type == 1) tar->addMoney(4);
		else tar->getCardFromHeap(4, 4);
	}
};

void getMoreResouce() {
	skill_regret = false;
	std::vector<std::string>choose{ "不额外获得资源","额外获得4枚金币","额外获得4张卡牌" };
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "是否额外获得资源", choose);
	int cidx = receiveInt(active_player);
	if (cidx == 1) {
		skill_regret = true;
		return;
	}
	getMoreResourceEffect* e = new getMoreResourceEffect;
	e->launch_player = active_player;
	e->target_player = active_player;
	e->resource_type = cidx - 1;
	e->stage = EffectStage::INSTANT;
	addEffect(e);
}

Pilot::Pilot() {
	setEName("pilot");
	setMaxBuildingTime(0);
	setRank(7);

	Skill s(getMoreResouce, LaunchStage::COLLECT);
	s.setName("航海");
	pushSkill(s);
}