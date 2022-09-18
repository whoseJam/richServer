
#include"administrator.h"
#include"autofactory.h"
#include"communicate.h"
#include"observer.h"
#include"gameflow.h"
#include"player.h"
#include"effect.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int skill_regret;
extern int tot_player;
extern int active_player;
extern Observer<std::function<void(int, Building*, bool&)>> administratorObserver;

autoFactory<Character>::registerClass<Administrator> regForAdministrator("administrator");

void Administrator::clear() {
	administratorObserver.stop();
	for (auto& it : skills) {
		it.setUsed(false);
	}
}

struct AdministratorObserverFunc {
	int bel, tar, r1, r2;
	void operator()(int pidx, Building* b, bool& isCaptured) {
		isCaptured = false;
		if (pidx == tar) {
			tar = 0;
			Player* my = findPlayer(bel);
			Player* tar = findPlayer(pidx);
			my->addBuilding(b);
			isCaptured = true;
		}
	}
};

void capturePlayer() {
	skill_regret = false;
	std::vector<std::string>choose;
	for (int i = 1; i <= tot_player; i++) {
		std::string tmp = "Player" + intToString(i);
		if (i == active_player)tmp = tmp + "(disabled)";
		choose.push_back(tmp);
	}
	sendLimitPrivateMessage(active_player, 2, "ÇëÑ¡ÔñÖÁ¶àÁ½¸öÎ´¸ÇÕÂ´þ²¶Áî", choose);
	std::vector<bool> result = receiveBoolVector(active_player);
	choose.clear();
	for (int i = 1; i <= tot_player; i++) {
		std::string tmp = "Player" + intToString(i);
		if (i == active_player || result[i - 1])tmp = tmp + "(disabled)";
		choose.push_back(tmp);
	}
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "ÇëÑ¡ÔñÒ»¸ö¸ÇÕÂ´þ²¶Áî", choose);
	int tidx = receiveInt(active_player);

	AdministratorObserverFunc func;
	func.bel = active_player;
	func.tar = tidx;
	administratorObserver.observe(func);
}

Administrator::Administrator() {
	setEName("administrator");
	setMaxBuildingTime(1);
	setRank(1);

	Skill s(capturePlayer, LaunchStage::ACTION);
	s.setName("´þ²¶Áî");
	pushSkill(s);
}