#pragma once

#include<functional>

enum class EffectStage {
	INSTANT,
	BEFORE_COLLECT,
	BEFORE_ACTION
};

class Effect {
public:
	int launch_player;
	int target_player;
	EffectStage stage;
	virtual void operator ()()=0 ;
};