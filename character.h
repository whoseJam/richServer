#pragma once

#include<functional>
#include<string>
#include<vector>

#include"skill.h"

class Player;
class Skill;

class Character {
public:
	int getRank();
	const std::vector<Skill>& getSkills();
	void output();
	int getMaxBuildingTime();
	void setName(std::string&&);
	void setEName(std::string&&);
	void setMaxBuildingTime(int);
	void setRank(int);
	void pushSkill(const Skill&);
	void popSkill();
	const std::string& getName();
	const std::string& getEName();
	virtual void clear();
	virtual void collect();
	void action();
	void sendMainChoice(int, int);
	virtual void sendCardToChoose(int);
	void activateSkill();
	virtual void activateBuild(int&);
	void launchSkillWhenCollecting();
	void launchSkillAfterCollecting();
	void launchSkillBeforeAction();
	void launchSkillAfterAction();
	int getSpecificSkillNum(LaunchStage);
protected:
	int rank;
	int maxBuildingTime;
	std::vector<Skill> skills;
	std::string name;
	std::string ename;
};