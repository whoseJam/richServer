#pragma once

#include<string>

enum BuildingType {
	RELIGION = 1,
	MILITARY = 2,
	COMMERCE = 4,
	NOBILITY = 8,
	SPECIAL = 16
};

class Building {
public:
	Building(int cost, BuildingType type, const std::string& name) :
		cost(cost), type(type), name(name) {}
	int getType();
	int getCost();
	void output();
	void setName(const std::string&);
	void setType(BuildingType);
	void setCost(int);
	const std::string& getName();
	virtual int getSymbolType();
	virtual int getValue();
	virtual void generateObserver(int bel);
	virtual void stopObserver();
private:
	std::string name;
	int cost;
	BuildingType type;
};