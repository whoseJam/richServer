
#include<iostream>
#include<memory>

#include"communicate.h"
#include"character.h"
#include"gameflow.h"
#include"observer.h"
#include"player.h"
#include"skill.h"
#include"tool.h"

extern int first_to_complete_building;
extern int tot_player;
extern int tot_character;
extern std::vector<Building*> cardHeap;;
extern Character* character[10];
extern Observer<std::function<void(int, int&)>> hauntedCityObserver;
extern Observer<std::function<void(int, int&)>> magicSchoolObserver;
extern Observer<std::function<void(int)>> laboratoryPushObserver;
extern Observer<std::function<void(int)>> laboratoryPopObserver;
extern Observer<std::function<void(int)>> workshopPushObserver;
extern Observer<std::function<void(int)>> workshopPopObserver;
extern void addCardHeap(Building*);

int Player::getSpecificBuildingNumsForMoney(int Set) {
	int ans = 0;
	for (auto& b : mBuilding) {
		if (b->getType() & Set)ans++;
	}
	magicSchoolObserver.trigger<int, int&>(idx, ans);
	return ans;
}

int Player::getSpecificBuildingNums(int Set) {
	int ans = 0;
	for (auto& b : mBuilding) {
		if (b->getType() & Set)ans++;
	}return ans;
}

int Player::getTotalBuildingNums() {
	return (int)mBuilding.size();
}

bool Player::findBuilding(const std::string& str) {
	for (auto& b : mBuilding) {
		if (b->getName() == str)return true;
	}return false;
}

void Player::chooseCharacter() {
	int c = sendCharacterToChoose(idx);
	mCharacter = character[c];
	character[c] = NULL;
	sendFlipCharacterPlayerStatus(idx, mCharacter->getEName());
}

int Player::getCharacterRank() {
	return mCharacter->getRank();
}

void Player::setSkipCollect(bool flg) {
	skipCollect = flg;
}

void Player::setSkipAction(bool flg) {
	skipAction = flg;
}

void Player::initForCardAndMoney() {
	money = 2;
	int f = (int)cardHeap.size();
	for (int i = 1; i <= 4; i++) {
		mCard.push_back(cardHeap[f - 1]);
		cardHeap.pop_back();
		f--;
	}
}

int Player::getScore() {
	int ans = 0;
	int building_set = 0;
	for (auto& it : mBuilding) {
		building_set |= it->getSymbolType();
		ans += it->getValue();
	}
	hauntedCityObserver.trigger<int, int&>(idx, building_set);
	int all = BuildingType::COMMERCE |
		BuildingType::MILITARY |
		BuildingType::NOBILITY |
		BuildingType::RELIGION |
		BuildingType::SPECIAL;
	if ((all & building_set) == all)ans += 3;
	if (getIsFirstToCompleteBuilding())ans += 4;
	if (getIsCompleteBuilding())ans += 2;
	return ans;
}

Character* Player::getCharacter() {
	return mCharacter;
}

void Player::pushBuildingSkill(const Skill& s) {
	mBuildingSkill.push_back(s);
}

void Player::popBuildingSkill() {
	if (mBuildingSkill.size())
		mBuildingSkill.pop_back();
	else {
		std::cout << "pop empty building skill\n";
		exit(0);
	}
}

std::vector<Skill>& Player::getBuildingSkill() {
	return mBuildingSkill;
}

void Player::getCardFromHeap(int tot, int select) {
	int f = (int)cardHeap.size();
	if (f < tot) { std::cout << "card is null\n"; exit(0); }
	std::vector<Building*>cardsSet;
	for (int i = 1; i <= tot; i++) {
		cardsSet.push_back(cardHeap[f - 1]);
		f--; cardHeap.pop_back();
	}
	for (int i = 1; i <= select; i++) {
		std::vector<std::string> choose;
		for (int i = 0; i < cardsSet.size(); i++)
			if (cardsSet[i] != NULL)choose.push_back(cardsSet[i]->getName() + "(building)");
		sendPrivateMessage(idx, PrivateMessageType::SINGLE_CHOICE, "请从" + intToString((int)choose.size()) + "张牌中选择" + intToString(select - i + 1) + "张牌", choose);
		int c = receiveInt(idx);
		addCard(cardsSet[c - 1]);
		cardsSet.erase(cardsSet.begin() + c - 1);
	}
	for (int i = 0; i < cardsSet.size(); i++) {
		addCardHeap(cardsSet[i]);
	}
	sendPublicMessage("Player" + intToString(idx) + "抽取了" + intToString(select) + "张牌");
}

void Player::act() {
	laboratoryPushObserver.trigger<int>(idx);
	workshopPushObserver.trigger<int>(idx);
	mxMoney = money;
	mnMoney = money;

	launchBeforeCollectEffect();
	if (!skipCollect) {
		mCharacter->launchSkillWhenCollecting();
		mCharacter->collect();
	}

	mCharacter->launchSkillAfterCollecting();
	launchBeforeActionEffect();
	if (!skipAction) {
		mCharacter->action();
	}
	mCharacter->launchSkillAfterAction();

	laboratoryPopObserver.trigger<int>(idx);
	workshopPopObserver.trigger<int>(idx);
}

int Player::getMoney() {
	return money;
}

int Player::getMaxMoneyInARound() {
	return mxMoney;
}

int Player::getMinMoneyInARound() {
	return mnMoney;
}

int Player::getSpecificCardNum(int Set) {
	int ans = 0;
	for (auto& c : mCard) {
		if (c->getType() & Set)ans++;
	}return ans;
}

int Player::getCardNum() {
	return (int)mCard.size();
}

void Player::setMoney(int m) {
	int del = m - money;
	int lst = money;
	money = m;
	sendModifyMoney(idx, lst, del);
}

void Player::addMoney(int m) {
	int lst = money;
	money += m;
	mxMoney = std::max(mxMoney, money);
	mnMoney = std::min(mnMoney, money);
	sendModifyMoney(idx, lst, m);
}

void Player::setIsCursed(bool flg) {
	isCursed = flg;
	if (isCursed)
		sendAddDebuff(idx, "cursedTag");
}

void Player::setIsKilled(bool flg) {
	isKilled = flg;
	if (isKilled)
		sendAddDebuff(idx, "killedTag");
}

void Player::setIsActived(bool flg) {
	isActived = flg;
	if (isActived)
		sendFlipCharacterCityLayout(idx, mCharacter->getEName());
}

void Player::setIsThieved(bool flg) {
	isThieved = flg;
	if (isThieved)
		sendAddDebuff(idx, "thievedTag");
}

void Player::setIsCompleteBuilding(bool flg) {
	isCompleteBuilding = flg;
	sendAddBuff(idx, "complete");
}

bool Player::getIsKilled() {
	return isKilled;
}

bool Player::getIsCursed() {
	return isCursed;
}

bool Player::getIsActived() {
	return isActived;
}

bool Player::getIsThieved() {
	return isThieved;
}

bool Player::getIsFirstToCompleteBuilding() {
	return isFirstToCompleteBuilding;
}

bool Player::getIsCompleteBuilding() {
	return isCompleteBuilding;
}

std::string Player::getName() {
	if (mCharacter == NULL)return "未知";
	return mCharacter->getName();
}

std::string Player::getEName() {
	if (mCharacter == NULL) return "mask";
	return mCharacter->getEName();
}

Building* Player::getBuilding(int idx) {
	return mBuilding[idx];
}

Building* Player::getCard(int idx) {
	return mCard[idx];
}

Building* Player::takeOutCard(int rk) {
	Building* p = mCard[rk];
	int lst = mCard.size();
	mCard.erase(mCard.begin() + rk);
	sendEraseCard(idx, rk);
	sendModifyCard(idx, lst, -1);
	return p;
}

Building* Player::takeOutRandomCard() {
	if (mCard.size() == 0)return NULL;
	int rk = Rand(0, mCard.size() - 1);
	return takeOutCard(rk);
}

void Player::addCard(Building* tmp) {
	int lst = mCard.size();
	mCard.push_back(tmp);
	sendAddCard(idx, tmp);
	sendModifyCard(idx, lst, 1);
}

void Player::eraseBuilding(int rk) {
	mBuilding[rk]->stopObserver();
	delete mBuilding[rk];
	mBuilding.erase(mBuilding.begin() + rk);
	sendEraseBuilding(idx, rk);
}

Building* Player::takeOutBuilding(int rk) {
	Building* ret = mBuilding[rk];
	mBuilding.erase(mBuilding.begin() + rk);
	ret->stopObserver();
	sendEraseBuilding(idx, rk);
	return ret;
}

void Player::clear() {
	sendClearDebuff(idx);
	if (mCharacter != NULL)
		mCharacter->clear();
	mCharacter = NULL;
	setIsActived(false);
	setSkipCollect(false);
	setSkipAction(false);
	setIsCursed(false);
	setIsKilled(false);
	setIsThieved(false);
	sendFlipCharacterPlayerStatus(idx, "mask");
	sendFlipCharacterCityLayout(idx, "mask");
}

void Player::setIsFirstToCompleteBuilding(bool flg) {
	sendAddBuff(idx, "first");
	isFirstToCompleteBuilding = flg;
}

void Player::addBuilding(Building* tmp) {
	mBuilding.push_back(tmp);
	sendAddBuilding(idx, tmp);
	tmp->generateObserver(idx);
	if (mBuilding.size() >= 7) {
		if (!first_to_complete_building) {
			first_to_complete_building = idx;
			setIsFirstToCompleteBuilding(true);
		}
		setIsCompleteBuilding(true);
	}
}