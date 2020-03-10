#pragma once

#include "Item.h"
#include "Stats.h"

const enum equipType { Armor, Boots, Gloves, Sword, Gun };

class Equipment : public Item
{
private:
	equipType type_;
	Stats modifiers_;

public:
	Equipment(equipType type) : Item(), type_(type) { modifiers_ = Stats(); };
	void writeStats();

#pragma region Setters
	void setHealth(int health) { modifiers_.health_ = health; };
	void setArmor(int armor) { modifiers_.armor_ = armor; };
	void setAd(int ad) { modifiers_.ad_ = ad; };
	void setAp(int ap) { modifiers_.ap_ = ap; };
	void setCrit(int crit) { modifiers_.crit_ = crit; };
	void setSpeed(int speed) { modifiers_.moveSpeed_ = speed; };
	void setDistRate(double distRate) { modifiers_.distRate_ = distRate; };
	void setMeleeRate(double meleeRate) { modifiers_.meleeRate_ = meleeRate; };
#pragma endregion

#pragma region Getters
	int getHealth() { return modifiers_.health_; };
	int getArmor() { return modifiers_.armor_; };
	int getAd() { return modifiers_.ad_; };
	int getAp() { return modifiers_.ap_; };
	int getCrit() { return modifiers_.crit_; };
	int getSpeed() { return modifiers_.moveSpeed_; };
	double getDistRate() { return modifiers_.distRate_; };
	double getMeleeRate() { return modifiers_.meleeRate_; };
#pragma endregion

};