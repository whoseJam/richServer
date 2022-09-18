
#include<iostream>

#include"skill.h"

LaunchStage Skill::getLaunchStage() {
	return stg;
}

void Skill::operator ()() {
	callback();
}

void Skill::setUsed(bool flg) {
	used = flg;
}

bool Skill::isUsed() {
	return used;
}

void Skill::setName(const std::string& str) {
	name = str;
}

const std::string& Skill::getName() {
	return name;
}