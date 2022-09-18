#pragma once

#include<functional>
#include<vector>

class Player;

enum class LaunchStage {
	COLLECT = 0,
	AFTER_COLLECT = 1,
	BEFORE_ACTION = 2,
	ACTION = 3,
	AFTER_ACTION = 4
};

class Skill {
public:
	using CallType = std::function<void()>;
	Skill(CallType callback, LaunchStage stg) :
		callback(callback), stg(stg), used(false) {}
	LaunchStage getLaunchStage();
	void operator ()();
	bool isUsed();
	void setUsed(bool flg);
	void setName(const std::string&);
	const std::string& getName();
private:
	std::string name;
	CallType callback;
	LaunchStage stg;
	bool used;
};