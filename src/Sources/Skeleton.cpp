﻿#include "Skeleton.h"
#include <iostream>
#include "Resources.h"
#include "BoneBullet.h"
#include "CollisionCtrl.h"
//#include "CaribbeanIslandState.h"

void Skeleton::updateAnim()
{
	if (currAnim_.frameRate_ <= SDL_GetTicks() - lasFrame_) {
		lasFrame_ = SDL_GetTicks();
		frame_ = SDL_Rect({ (int)pos_.getX(),(int)pos_.getY(),(int)currAnim_.widthFrame_,(int)currAnim_.heightFrame_ });
	}
}

void Skeleton::initialStats() {
	HEALTH = 100;
	MANA = 100;
	MANA_REG = 1;
	ARMOR = 10;
	MELEE_DMG = 0;
	DIST_DMG = 100;
	CRIT = 0;
	MELEE_RANGE = 20;
	DIST_RANGE = 250;
	MOVE_SPEED = 100;
	MELEE_RATE = 1;
	DIST_RATE = 2500;
	initStats(HEALTH, MANA, MANA_REG, ARMOR, MELEE_DMG, DIST_DMG, CRIT, MELEE_RANGE, DIST_RANGE, MOVE_SPEED, MELEE_RATE, DIST_RATE);
}


void Skeleton::attack() {
	if (currStats_.distRate_ <= SDL_GetTicks() - lastHit) {
		lastHit = SDL_GetTicks();
		Vector2D dir = Vector2D(currEnemy_->getPosX() + (currEnemy_->getScaleX() / 2), currEnemy_->getPosY() + (currEnemy_->getScaleY() / 2));
		BoneBullet* bone = new BoneBullet(app_, app_->getTextureManager()->getTexture(Resources::Coco),
			getCenter(), dir, currStats_.distDmg_, BONE_LIFE, BONE_VEL, Vector2D(BONE_WIDTH, BONE_HEIGHT));
		app_->getCurrState()->addRenderUpdateLists(bone);
		CollisionCtrl::instance()->addEnemyBullet(bone);
	}
}

void Skeleton::initObject() {
	setTexture(app_->getTextureManager()->getTexture(Resources::Skeleton));
	initStats(HEALTH, MANA, MANA_REG, ARMOR, MELEE_DMG, DIST_DMG, CRIT, MELEE_RANGE, DIST_RANGE, MOVE_SPEED, MELEE_RATE, DIST_RATE);
	destiny_ = SDL_Rect({ (int)pos_.getX(),(int)pos_.getX(),(int)scale_.getX(),(int)scale_.getY() });
	scaleCollision_.setVec(Vector2D(scale_.getX(), scale_.getY()));
	collisionArea_ = SDL_Rect({ (int)pos_.getX(),(int)pos_.getY(),(int)scaleCollision_.getX(),(int)scaleCollision_.getY() });
	initAnims();
}

void Skeleton::lostAggro()
{
	currEnemy_ = nullptr;
}

bool Skeleton::update() {

	
	//Si el esqueleto ha muerto
	if (currState_ == STATE::DYING) {
		// animación de muerte si la tiene
		//Cuando acabe la animación, lo mata
		applyRewards();
		app_->getCurrState()->removeRenderUpdateLists(this);
		return true;
	}
	//Si el esqueleto no tiene enemigo al atacar, elige enemigo teniendo prioridad sobre el enemigo más cercano
	if (currState_ == STATE::IDLE && getEnemy(currStats_.distRange_)) {
		currState_ = STATE::ATTACKING;
	}
	//Si el esqueleto tiene enemigo y puede atacar
	if (currState_ == STATE::ATTACKING && currStats_.distRate_ <= SDL_GetTicks() - lastHit) {
		//Si el esqueeto tiene un enemigo y lo tiene a rango
		if (onRange()) {
			//changeAnim(attackAnim_);//animacion de ataque del esqueleto
			attack();
		}
		//Tengo enemigo como objetivo, pero no a rango, busco si hay otro cerca para atacar
		else if (getEnemy(currStats_.distRange_))
		{
			//changeAnim(attackAnim_);//animacion de ataque del esqueleto
			attack();
		}
		//Tengo enemigo pero no a rango
		else
		{
			currState_ == STATE::IDLE;
			//changeAnim(idleAnim_); animacion de idle del esqueleto
			currEnemy_ = nullptr;
		}
		lastHit = SDL_GetTicks();
	}

	updateAnim();
	return false;
}

void Skeleton::initRewards()
{
	minGold = 30;
	maxGold = 50;
	minArchievementPoints = 2;
	maxArchievementPoints = 10;
	goldPoints_ = app_->getRandom()->nextInt(minGold, maxGold + 1);
	achievementPoints_ = app_->getRandom()->nextInt(minArchievementPoints, maxArchievementPoints + 1);
}

void Skeleton::initAnims() 
{
	//Animaci�n de idle
		//Arriba
	idleAnims_.push_back(Anim(IDLE_U_FRAMES, W_SKELETON_FRAME, H_SKELETON_FRAME, IDLE_U_FRAME_RATE, true));
	idleTx_.push_back(app_->getTextureManager()->getTexture(Resources::SkeletonIdleUpAnim));
	//Derecha																						
	idleAnims_.push_back(Anim(IDLE_R_FRAMES, W_SKELETON_FRAME, H_SKELETON_FRAME, IDLE_R_FRAME_RATE, true));
	idleTx_.push_back(app_->getTextureManager()->getTexture(Resources::SkeletonIdleRightAnim));
	//Abajo																							
	idleAnims_.push_back(Anim(IDLE_D_FRAMES, W_SKELETON_FRAME, H_SKELETON_FRAME, IDLE_D_FRAME_RATE, true));
	idleTx_.push_back(app_->getTextureManager()->getTexture(Resources::SkeletonIdleDownAnim));
	//Izquierda																						
	idleAnims_.push_back(Anim(IDLE_L_FRAMES, W_SKELETON_FRAME, H_SKELETON_FRAME, IDLE_L_FRAME_RATE, true));
	idleTx_.push_back(app_->getTextureManager()->getTexture(Resources::SkeletonIdleLeftAnim));

	//Inicializamos con la animación del idle
	currDir_ = DIR::DOWN;
	initIdle();
}

void Skeleton::initIdle()
{
	currState_ = STATE::IDLE;
	texture_ = idleTx_[(int)currDir_];
	currAnim_ = idleAnims_[(int)currDir_];
	frame_.x = 0; frame_.y = 0;
	frame_.w = currAnim_.widthFrame_;
	frame_.h = currAnim_.heightFrame_;
}