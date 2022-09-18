
#include"autofactory.h"
#include"communicate.h"
#include"gameflow.h"
#include"observer.h"
#include"scholar.h"
#include"effect.h"
#include"player.h"
#include"tool.h"

extern Player* findPlayer(int);
extern int active_player;
extern Observer<std::function<void(int, bool&)>> libraryObserver;
extern Observer<std::function<void(int, bool&)>> observatoryObserver;

autoFactory<Character>::registerClass<Scholar> regForScholar("scholar");

void Scholar::collect() {
	//Send information
	std::vector<std::string> choose = { "��ý��","����" };
	sendPrivateMessage(active_player, PrivateMessageType::SINGLE_CHOICE, "�ɼ���Դ��...��ѡ��", choose);
	sendPublicMessage("Player" + intToString(active_player) + "����ѡ��ɼ�ʲô����...");
	int c = receiveInt(active_player);
	if (c == 1) {
		sendPublicMessage("Player" + intToString(active_player) + "ѡ������ö���");
		Player* p = findPlayer(active_player);
		p->addMoney(2);
	}
	else {
		sendPublicMessage("Player" + intToString(active_player) + "ѡ���ȡ����");
		Player* p = findPlayer(active_player);
		bool needGetCard = true;
		if (needGetCard)observatoryObserver.trigger<int, bool&>(active_player, needGetCard);
		if (needGetCard)libraryObserver.trigger<int, bool&>(active_player, needGetCard);
		if (needGetCard)p->getCardFromHeap(7, 1);
	}
	sendPrivateMessage(active_player, "��ƽ�˾�...");
}

Scholar::Scholar() {
	setName("ѧ��");
	setEName("scholar");
	setMaxBuildingTime(2);
	setRank(7);
}