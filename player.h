#pragma once

#include<vector>
#include<memory>
#include"character.h"
#include"building.h"

class Player {
public:
	void act();
	void initForCardAndMoney();
	int getSpecificBuildingNumsForMoney(int);
	int getSpecificBuildingNums(int);
	int getTotalBuildingNums();
	void chooseCharacter();
	int getCharacterRank();
	int getMoney();
	int getSpecificCardNum(int);
	int getCardNum();
	int getScore();
	bool findBuilding(const std::string&);
	bool getIsKilled();
	bool getIsCursed();
	bool getIsActived();
	bool getIsThieved();
	bool getIsFirstToCompleteBuilding();
	bool getIsCompleteBuilding();
	void setMoney(int m);
	void addMoney(int m);
	void addBuilding(Building*);
	void addCard(Building*);
	void setSkipCollect(bool);
	void setSkipAction(bool);
	void setIsKilled(bool);
	void setIsCursed(bool);
	void setIsActived(bool);
	void setIsThieved(bool);
	void setIsFirstToCompleteBuilding(bool);
	void setIsCompleteBuilding(bool);
	void eraseBuilding(int);
	void pushBuildingSkill(const Skill&);
	void popBuildingSkill();
	std::string getName();
	std::string getEName();
	void getCardFromHeap(int tot, int select);
	Building* getBuilding(int idx);
	Building* takeOutBuilding(int idx);
	Building* takeOutCard(int);
	Building* takeOutRandomCard();
	Building* getCard(int idx);
	Character* getCharacter();
	std::vector<Skill>& getBuildingSkill();
	void clear();
	int getMaxMoneyInARound();
	int getMinMoneyInARound();
private:
	int mxMoney;
	int mnMoney;

	Character* mCharacter;
	std::vector<Building*> mCard;
	std::vector<Building*> mBuilding;
	std::vector<Skill> mBuildingSkill;
	int money;
	bool skipCollect;
	bool skipAction;
	bool isKilled;
	bool isCursed;
	bool isActived;
	bool isThieved;
	bool isFirstToCompleteBuilding;
	bool isCompleteBuilding;
public:
	int idx;
};