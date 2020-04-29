#include "MonkeyCoco.h"
#include <iostream>
#include "Resources.h"
#include "Bullet.h"
#include "CollisionCtrl.h"
#include "CaribbeanIslandState.h"
#include <string>

bool MonkeyCoco::update() {

	updateFrame();
	//Si el mono ha muerto
	if (currState_ == STATE::DYING) {
		//Tendr�a que hacer la animaci�n de muerte?
		//Cuando acabe la animaci�n, lo mata
		applyRewards();
		app_->getCurrState()->removeRenderUpdateLists(this);
		return true;
	}
	//Si el mono no tiene enemigo al atacar, elige enemigo teniendo prioridad sobre el enemigo m�s cercano
	if (currState_ == STATE::IDLE && getEnemy(currStats_.distRange_)) {
		currState_ = STATE::ATTACKING;
	}
	//Si el mono tiene enemigo y puede atacar
	if (currState_ == STATE::ATTACKING && currStats_.distRate_ <= SDL_GetTicks() - lastHit) {
		//Si el mono tiene un enemigo y lo tiene a rango
		if (onRange()) {
			//changeAnim(attackAnim_);
			attack();
		}
		//Tengo enemigo como objetivo, pero no a rango, busco si hay otro cerca para atacar
		else if(getEnemy(currStats_.distRange_))
		{
			//changeAnim(attackAnim_);
			attack();
		}
		//Tengo enemigo pero no a rango
		else
		{
			currState_ = STATE::IDLE;
			currEnemy_ = nullptr;
		}
		lastHit = SDL_GetTicks();
	}
	return false;
}


//Inicializa todas las animaciones
void MonkeyCoco::initAnims()
{
	//Para la animaci�n de ataque
	attackAnim_ = Anim(NUM_FRAMES_ATK, W_FRAME_ATK, H_FRAME_ATK, FRAME_RATE_ATK, false);
	//Para la animaci�n de caminar
	walkAnim_ = Anim(NUM_FRAMES_MOV, W_FRAME_MOV, H_FRAME_MOV, FRAME_RATE_MOV, true);
	//Para la animaci�n de parado
	idleAnim_ = Anim(NUM_FRAMES_IDLE, W_FRAME_IDLE, H_FRAME_IDLE, FRAME_RATE_IDLE, true);
}

//Se encarga de crear el coco en direcci�n al enemigo
void MonkeyCoco::attack() {
	Vector2D dir = Vector2D(currEnemy_->getPosX() + (currEnemy_->getScaleX() / 2), currEnemy_->getPosY() + (currEnemy_->getScaleY() / 2));
	Bullet* coco = new Bullet(app_, app_->getTextureManager()->getTexture(Resources::Coco),
		getCenter(), dir, currStats_.distDmg_, COCO_LIFE, COCO_VEL, Vector2D(W_H_COCO, W_H_COCO));
	app_->getCurrState()->addRenderUpdateLists(coco);
	CollisionCtrl::instance()->addEnemyBullet(coco);
}

//Inicializa al mono
void MonkeyCoco::initObject() {
	Enemy::initObject();
	setTexture(app_->getTextureManager()->getTexture(Resources::MonkeyFront));
	initAnims();
	initRewards();
}

void MonkeyCoco::initialStats()
{
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

//Gesti�n de las colisiones
void MonkeyCoco::onCollider() {};

void MonkeyCoco::initRewards()
{
	minGold = 30;
	maxGold = 50;
	minArchievementPoints = 2;
	maxArchievementPoints = 10;
	goldPoints_ = app_->getRandom()->nextInt(minGold, maxGold + 1);
	achievementPoints_ = app_->getRandom()->nextInt(minArchievementPoints, maxArchievementPoints + 1);
}
