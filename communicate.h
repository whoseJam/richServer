#pragma once

#include<vector>
#include<string>

class Character;
class Building;

enum PrivateMessageType {
	SINGLE_CHOICE,
	MULTIPLE_CHOICE,
	LIMIT_MULTIPLE_CHOICE,
	CONST_MULTIPLE_CHOICE
};

void initCommunicate();

int sendCharacterToChoose(int cdx);

void sendInitCityLayout();
void sendFlipCharacterCityLayout(int pidx, const std::string&);
void sendFlipCharacterPlayerStatus(int pidx, const std::string&);
void sendAddBuilding(int pidx, Building*);
void sendEraseBuilding(int pidx, int bidx);
void sendModifyMoney(int pidx, int bef, int delta);
void sendModifyCard(int pidx, int bef, int delta);
void sendAddBuff(int pidx, const std::string&);
void sendAddDebuff(int pidx, const std::string&);
void sendClearDebuff(int pidx);
void sendAddCard(int pidx, Building*);
void sendEraseCard(int pidx, int cidx);
void sendPlayerIdx(int pidx);

void sendPrivateMessage(int cdx, const std::string& msg);
void sendPrivateMessage(int cdx, PrivateMessageType, const std::string& msg, std::vector<std::string>&);
void sendLimitPrivateMessage(int cdx, int limit, const std::string& msg, std::vector<std::string>&);
void sendConstPrivateMessage(int cdx, int limit, const std::string& msg, std::vector<std::string>&);
void sendPublicMessage(const std::string& msg);

int receiveInt(int cdx);
std::vector<bool> receiveBoolVector(int cdx);
void destroyCommunicate();