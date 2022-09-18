
#include<vector>
#include<iostream>

#include"gameflow.h"
#include"observer.h"
#include"player.h"
#include"tool.h"

extern int active_player;

std::vector<Effect*> effects;

void addEffect(Effect* effect) {
	if (effect->stage == EffectStage::INSTANT) {
		std::cout << "launch Effect Instant\n";
		(*effect)(); delete effect;
	}
	else effects.push_back(effect);
}
bool tryLaunchEffect(EffectStage stage) {
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i]->stage == stage && effects[i]->target_player == active_player) {
			(*(effects[i]))();
			delete effects[i];
			effects.erase(effects.begin() + i);
			return true;
		}
	}return false;
}
void launchBeforeCollectEffect() {
	while (tryLaunchEffect(EffectStage::BEFORE_COLLECT));
}
void launchBeforeActionEffect() {
	while (tryLaunchEffect(EffectStage::BEFORE_ACTION));
}

std::vector<Building*> cardHeap;

int target_player;
int active_player;

Character* character[10];
Character* tmpCharacter[10];
int tot_character;

Player players[10];
int tot_player;
int skill_regret;
int first_to_complete_building;

Player* findPlayerWithCharacter(int idx) {
	for (int i = 1; i <= tot_player; i++) {
		if (players[i].getCharacterRank() == idx)
			return &players[i];
	}
	return nullptr;
}

Player* findPlayer(int idx) {
	return &players[idx];
}

Character* findCharacter(int idx) {
	for (int i = 1; i <= tot_character; i++) {
		if (tmpCharacter[i]->getRank() == idx)
			return tmpCharacter[i];
	}
}

bool gameOver() {
	for (int i = 1; i <= tot_player; i++) {
		if (players[i].getTotalBuildingNums() >= 7)
			return true;
	}return false;
}

void mixCharacter() {
	for (int i = 1; i <= tot_character; i++) {
		character[i] = tmpCharacter[i];
		std::swap(character[i], character[Rand(1, i)]);
	}
	for (int i = 1; i <= tot_character; i++) {
		if (character[i]->getRank() == 1) {
			std::swap(character[i], character[1]);
		}
	}
	for (int i = 1; i <= tot_character; i++) {
		tmpCharacter[i] = character[i];
	}
}

void mixCardHeap() {
	for (int i = 0; i < cardHeap.size(); i++) {
		int idx = Rand(0, i);
		std::swap(cardHeap[i], cardHeap[idx]);
	}
}

void addCardHeap(Building* b) {
	cardHeap.push_back(b);
	mixCardHeap();
}

Observer<std::function<void(Building*, bool&)>> warloadDestroyAnBuilding;
Observer<std::function<void(int, bool&)>> libraryObserver;
Observer<std::function<void(int, int&)>> hauntedCityObserver;
Observer<std::function<void(int, int&)>> magicSchoolObserver;

Observer<std::function<void(int)>> laboratoryPushObserver;
Observer<std::function<void(int)>> laboratoryPopObserver;
Observer<std::function<void(int)>> workshopPushObserver;
Observer<std::function<void(int)>> workshopPopObserver;

Observer<std::function<void(int, bool&)>> observatoryObserver;
Observer<std::function<void(int, Building*, bool&)>> keepObserver;
Observer<std::function<void(int, Building*, int&)>> greatWallObserver;
Observer<std::function<void()>>threaterObserver;

Observer<std::function<void(int, Building*, bool&)>>administratorObserver;