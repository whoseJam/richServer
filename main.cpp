#include<iostream>
#include<vector>

#include"collection.h"

extern std::vector<Building*> cardHeap;
extern int target_player;
extern int active_player;
extern int tot_character;
extern int tot_player;
extern Character* tmpCharacter[10];
FILE* config;

extern Player* findPlayerWithCharacter(int);
extern Player* findPlayer(int);
extern void mixCardHeap();
extern void mixCharacter();
extern bool gameOver();

void readConfig() {
	FILE* f = fopen("cards.txt", "r");
	while (true) {
		std::string name;
		std::string cost;
		std::string type;
		std::string size;
		int flg = readItem(f, name);
		if (flg == false)break;
		readItem(f, cost); int icost = stringToInt(cost);
		readItem(f, type); BuildingType itype;
		if (type == "RELIGION")itype = BuildingType::RELIGION;
		if (type == "COMMERCE")itype = BuildingType::COMMERCE;
		if (type == "NOBILITY")itype = BuildingType::NOBILITY;
		if (type == "MILITARY")itype = BuildingType::MILITARY;
		readItem(f, size); int isize = stringToInt(size);

		while (isize--)
			cardHeap.push_back(new Building(icost, itype, name));
	}
	int nsize;
	char buf[20];
	fscanf(config, "%d", &nsize);
	for (int i = 1; i <= nsize; i++) {
		fscanf(config, "%s", buf);
		cardHeap.push_back(autoFactory<Building>::getNormalPtr(buf));
	}
	fscanf(config, "%d", &tot_character);
	for (int i = 1; i <= tot_character; i++) {
		fscanf(config, "%s", buf);
		tmpCharacter[i] = autoFactory<Character>::getNormalPtr(buf);
	}
	mixCardHeap();
	mixCardHeap();
	mixCharacter();
	mixCharacter();
}

int main() {
	srand(time(0));
	std::string configName;
	std::cout << "请输入配置文件:\n";
	//std::cin >> configName;
	configName = "debug";
	config = fopen(configName.data(), "r");
	readConfig();
	target_player = 1;
	std::cout << "请输入玩家个数:\n";
	std::cin >> tot_player;
	//tot_player = 4;
	for (int i = 1; i <= tot_player; i++) {
		Player* p = findPlayer(i);
		p->idx = i;
		p->initForCardAndMoney();
	}
	initCommunicate();

	sendInitCityLayout();
	for (int i = 1; i <= tot_player; i++) {
		Player* my = findPlayer(i);
		for (int j = 0; j < 4; j++) {
			sendAddCard(i, my->getCard(j));
		}
		sendModifyMoney(i, 0, 2);
		sendModifyCard(i, 0, 4);
		sendPlayerIdx(i);
	}
	while (true) {
		for (int i = 1; i <= tot_player; i++)
			findPlayer(i)->clear();

		mixCardHeap();
		mixCardHeap();
		mixCharacter();
		mixCharacter();

		for (int i = target_player, j = 1; j <= tot_player; i = nxt(i, tot_player), j++)
			findPlayer(i)->chooseCharacter();

		for (int i = 1; i <= tot_character; i++) {
			Player* p = findPlayerWithCharacter(i);
			if (p != nullptr) {
				active_player = p->idx;
				p->setIsActived(true);

				sendPublicMessage("Player" + intToString(p->idx) + "的回合！");
				if (p->getIsKilled())sendPublicMessage("Player" + intToString(p->idx) + "被发现倒在血泊中，回合跳过！");
				for (int j = 1; j <= tot_player; j++)
					sendPrivateMessage(j, "风平浪静...");

				p->act();
			}
		}

		for (int i = 1; i <= tot_player; i++)
			sendPrivateMessage(i, "风平浪静...");

		if (gameOver())break;
	}

	for (int i = 1; i <= tot_player; i++) {
		std::string result = "Player" + intToString(i) + ":" + intToString(findPlayer(i)->getScore());
		sendPublicMessage(result);
	}
	return 0;
}
