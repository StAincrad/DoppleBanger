﻿#include "Clon.h"
#include "PlayState.h"
#include "Player.h"
#include "GameManager.h"
#include "PlayerBullet.h"

bool Clon::update()
{
	if ((SDL_GetTicks() - spawnTime_) / 1000 < duration_) {
		if (meleeDmg_ > 0 && (objective_ == nullptr || objective_->getState() == STATE::DYING || Vector2D(abs(objective_->getPosX() - pos_.getX()), abs(objective_->getPosY() - pos_.getY())).magnitude() > range_))
			objective_ = static_cast<PlayState*>(app_->getGameStateMachine()->getState())->findClosestEnemy(pos_);

		else if (meleeDmg_ > 0 && ((SDL_GetTicks() - meleeTime_) / 1000) > meleeRate_)
		{
			objective_->receiveDamage(meleeDmg_);
			meleeTime_ = SDL_GetTicks();
		}
	}
	else if (alive) player_->killClon();

	return false;
}

void Clon::initObject() {
	GameManager::instance()->setClon(this);
	texture_ = app_->getTextureManager()->getTexture(Resources::PlayerFront);
	spawnTime_ = SDL_GetTicks();
	duration_ = DURATION_;
	meleeRate_ = (player_->getStats().meleeRange_ / 2) * player_->getLiberation();
	meleeDmg_ = (player_->getStats().meleeDmg_ / 2) * player_->getLiberation();
	distDmg_ = (player_->getStats().distDmg_ / 2) * player_->getLiberation();
}

void Clon::shoot(Vector2D dir)
{
	if (distDmg_ > 0)
	{
		//Se calcula la posici�n desde la cual se dispara la bala
		Vector2D shootPos;
		shootPos.setX(pos_.getX() + (scale_.getX() / 2));
		shootPos.setY(pos_.getY() + (scale_.getY() / 2));

		PlayerBullet* bullet = new PlayerBullet(app_, app_->getTextureManager()->getTexture(Resources::Rock),
			shootPos, dir, distDmg_);

		//Se añade a los bucles del juegos
		app_->getCurrState()->addRenderUpdateLists(bullet);
		CollisionCtrl::instance()->addPlayerBullet(bullet);
	}
}

void Clon::die()
{
	GameManager* gm = GameManager::instance();
	for (auto enemy : agredEnemys_) {
		enemy->lostAgro();
	}
	agredEnemys_.clear();
	gm->setClon(nullptr);
	app_->getGameStateMachine()->getState()->removeRenderUpdateLists(this);
	alive = false;
}