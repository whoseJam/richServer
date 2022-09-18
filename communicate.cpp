
#include"communicate.h"
#include"sockbuffer.h"
#include"character.h"
#include"building.h"
#include"player.h"
#include"reader.h"
#include"tool.h"

#include<iostream>
#include<WinSock2.h>
#pragma comment (lib,"ws2_32.lib")

Reader comReader;

extern Player players[10];
extern Character* character[10];
extern int tot_character;
extern int tot_player;
extern int active_player;
extern Player* findPlayer(int);

WSAData wsaData;
SOCKET servSock;

SOCKET clntSock[10];
SockBuffer sockBuffer[10];

void initCommunicate() {
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSock == INVALID_SOCKET) { std::cout << "Socket Error\n"; exit(0); }
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	if (bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) { std::cout << "Bind Error\n"; exit(0); }
	if (listen(servSock, 20) == SOCKET_ERROR) { std::cout << "Listen Error\n"; exit(0); }
	for (int i = 1; i <= tot_player; i++) {
		SOCKADDR clntAddr;
		int nSize = sizeof(SOCKADDR);
		clntSock[i] = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		std::cout << "One Client is linking...\n";
		std::cout << "Total Client=" << i << "\n";
		if (clntSock[i] == INVALID_SOCKET) {
			std::cout << "Accept Error\n";
			exit(0);
		}
		sockBuffer[i].sock = clntSock[i];
	}
}

void sendInitCityLayout() {
	std::string order = "3";
	std::string buf = "InitCityLayout," + intToString(tot_player);
	for (int i = 1; i <= tot_player; i++) {
		sockBuffer[i].sendString(order);
		sockBuffer[i].sendString(buf);
	}
}

int sendCharacterToChoose(int cdx) {
	int res = 0;
	std::vector<std::string> choose;
	std::vector<int> pos;
	for (int i = 1; i <= tot_player; i++) {
		if (character[i] != NULL) {
			choose.push_back(character[i]->getEName() + "(character)");
			pos.push_back(i);
			res++;
		}
	}
	if (res == 1) {
		choose.push_back(character[tot_player + 1]->getEName() + "(character)");
		pos.push_back(tot_player + 1);
	}
	sendPrivateMessage(cdx, PrivateMessageType::SINGLE_CHOICE, "请选择你的角色", choose);
	int c = receiveInt(cdx);
	int p = pos[c - 1];
	return p;
}

void sendFlipCharacterCityLayout(int pidx, const std::string& chname) {
	std::string order = "3";
	Player* my = findPlayer(pidx);
	std::string buf = "FlipCharacter," + intToString(pidx) + "," + chname;
	for (int i = 1; i <= tot_player; i++) {
		sockBuffer[i].sendString(order);
		sockBuffer[i].sendString(buf);
	}
}
void sendFlipCharacterPlayerStatus(int pidx, const std::string& chname) {
	std::string order = "4";
	std::string buf = "FlipCharacter," + chname;
	sockBuffer[pidx].sendString(order);
	sockBuffer[pidx].sendString(buf);
}
void sendAddBuilding(int pidx, Building* b) {
	std::string order = "3";
	std::string buf = "AddBuilding," + intToString(pidx) + "," + b->getName();
	for (int i = 1; i <= tot_player; i++) {
		sockBuffer[i].sendString(order);
		sockBuffer[i].sendString(buf);
	}
}
void sendEraseBuilding(int pidx, int bidx) {
	std::string order = "3";
	std::string buf = "EraseBuilding," + intToString(pidx) + "," + intToString(bidx);
	for (int i = 1; i <= tot_player; i++) {
		sockBuffer[i].sendString(order);
		sockBuffer[i].sendString(buf);
	}
}
void sendAddCard(int pidx, Building* b) {
	std::string order = "5";
	std::string buf = "AddCard," + (b->getName());
	sockBuffer[pidx].sendString(order);
	sockBuffer[pidx].sendString(buf);

	//order="3";
	//sockBuffer[pidx].sendString(order);
	//sockBuffer[pidx].sendString();
}
void sendEraseCard(int pidx, int cidx) {
	std::string order = "5";
	std::string buf = "EraseCard," + intToString(cidx);
	sockBuffer[pidx].sendString(order);
	sockBuffer[pidx].sendString(buf);
}
void sendModifyMoney(int pidx, int bef, int delta) {
	std::string order = "3";
	std::string buf = "ModifyMoney," + intToString(pidx) + "," + intToString(bef) + "," + intToString(delta);
	for (int i = 1; i <= tot_player; i++) {
		sockBuffer[i].sendString(order);
		sockBuffer[i].sendString(buf);
	}
}
void sendModifyCard(int pidx, int bef, int delta) {
	std::string order = "3";
	std::string buf = "ModifyCard," + intToString(pidx) + "," + intToString(bef) + "," + intToString(delta);
	for (int i = 1; i <= tot_player; i++) {
		sockBuffer[i].sendString(order);
		sockBuffer[i].sendString(buf);
	}
}
void sendAddDebuff(int pidx, const std::string& debuff) {
	std::string order = "4";
	std::string buf = "AddDebuff," + debuff;
	sockBuffer[pidx].sendString(order);
	sockBuffer[pidx].sendString(buf);
}
void sendAddBuff(int pidx, const std::string& buff) {
	std::string order = "4";
	std::string buf = "AddBuff," + buff;
	sockBuffer[pidx].sendString(order);
	sockBuffer[pidx].sendString(buf);
}
void sendClearDebuff(int pidx) {
	std::string order = "4";
	std::string buf = "ClearDebuff";
	sockBuffer[pidx].sendString(order);
	sockBuffer[pidx].sendString(buf);
}
void sendPlayerIdx(int pidx) {
	std::string order = "4";
	std::string buf = "SetPlayerIdx," + intToString(pidx);
	sockBuffer[pidx].sendString(order);
	sockBuffer[pidx].sendString(buf);
}

void sendPublicMessage(const std::string& msg) {
	std::string str = msg + ",nextline";
	std::string order = "6";
	for (int i = 1; i <= tot_player; i++) {
		sockBuffer[i].sendString(order);
		sockBuffer[i].sendString(str);
	}
}

void sendPrivateMessage(int cdx, const std::string& msg) {
	std::string str = "0," + msg + ",nextline";
	std::string order = "7";
	sockBuffer[cdx].sendString(order);
	sockBuffer[cdx].sendString(str);
}

void sendPrivateMessage(int cdx, PrivateMessageType t, const std::string& msg, std::vector<std::string>& choose) {
	std::string str = intToString(t) + "," + msg + ",";
	for (auto& it : choose) str = str + it + ",";
	str = str + "nextline";
	std::string order = "7";
	sockBuffer[cdx].sendString(order);
	sockBuffer[cdx].sendString(str);
}
void sendLimitPrivateMessage(int cdx, int limit, const std::string& msg, std::vector<std::string>& choose) {
	std::string str = intToString(PrivateMessageType::LIMIT_MULTIPLE_CHOICE) + "," + intToString(limit) + "," + msg + ",";
	for (auto& it : choose) str = str + it + ",";
	str = str + "nextline";
	std::string order = "7";
	sockBuffer[cdx].sendString(order);
	sockBuffer[cdx].sendString(str);
}
void sendConstPrivateMessage(int cdx, int limit, const std::string& msg, std::vector<std::string>& choose) {
	std::string str = intToString(PrivateMessageType::CONST_MULTIPLE_CHOICE) + "," + intToString(limit) + "," + msg + ",";
	for (auto& it : choose) str = str + it + ",";
	str = str + "nextline";
	std::string order = "7";
	sockBuffer[cdx].sendString(order);
	sockBuffer[cdx].sendString(str);
}


int receiveInt(int cdx) {
	std::string buf = sockBuffer[cdx].readString();
	comReader.init(buf);
	return comReader.readInt();
}

std::vector<bool> receiveBoolVector(int cdx) {
	std::string buf = sockBuffer[cdx].readString();
	comReader.init(buf);
	std::vector<bool> ans;
	while (comReader.isEmpty() == false)
		ans.push_back(comReader.readInt());
	return ans;
}

void destroyCommunicate() {
	for (int i = 1; i <= tot_player; i++)
		closesocket(clntSock[i]);
	closesocket(servSock);
	WSACleanup();
}