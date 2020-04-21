#include "Gun.h"
#include "Player.h"

void Gun::equip(Player* player)
{
	player->addMeleeDmg(ad_);
	player->addDistRate(distRate_);
}

void Gun::remove(Player* player)
{
	player->removeAd(ad_);
	player->removeDistRate(distRate_);
}

void Gun::initStats()
{
	modifiers_.crit_ = crit_;
	modifiers_.distDmg_ = ad_;
	modifiers_.distRange_ = disRange_;
	modifiers_.distRate_ = distRate_;
}
