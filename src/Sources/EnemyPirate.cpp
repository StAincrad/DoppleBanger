#include "EnemyPirate.h"
#include "GameManager.h"
#include "Bullet.h"
#include "CollisionCtrl.h"
#include "CaribbeanIslandState.h"
#include <string>

bool EnemyPirate::update() {
#ifdef _DEBUG
	/*string state;
	switch (currAtackStatus_)
	{
	case ATK_STATUS::MELEE:
		state = "melee";
		break;
	case ATK_STATUS::RANGE:
		state = "range";
	default:
		break;
	}
	/*switch (currState_)
	{
	case STATE::ATTACKING:
		state = "atacando";
		break;
	case STATE::DYING:
		state = "muriendo";
		break;
	case STATE::FOLLOWING:
		state = "siguiendo";
		break;
	case STATE::IDLE:
		state = "parado";
		break;
	case STATE::PATROLLING:
		state = "patrullando";
		break;
	default:
		break;
	}*/
#endif // _DEBUG

	updateFrame();
	//Si el pirata ha muerto
	if (currState_ == STATE::DYING) {
		//Tendría que hacer la animación de muerte?
		//CollisionCtrl::instance()->addEnemiesToErase(this);
		app_->getCurrState()->removeRenderUpdateLists(this);
		return true;
	}
	//Si el pirata no tiene enemigo al atacar, elige enemigo teniendo prioridad sobre el enemigo más cercano
	if ((currState_ == STATE::IDLE || currState_ == STATE::PATROLLING) && getEnemy()) {
		currState_ = STATE::ATTACKING;
	}
	//Si el pirata tiene enemigo y puede atacar
	if (currState_ == STATE::ATTACKING) {
		//Si el pirata tiene un enemigo y lo tiene a rango
		if (onRange()) {
			//changeAnim(attackAnim_);
			attack();
		}
		else
		{
			currState_ = STATE::FOLLOWING;
			//changeAnim(walkAnim_);
			selectTarget();
		}
	}
	//Si el pirata se está moviendo hacia un enemigo
	if (currState_ == STATE::FOLLOWING) {
		if (onRange()) {
			currState_ = STATE::ATTACKING;
			stop();
			attack();
		}
		else
		{
			//changeAnim(walkAnim_);
			selectTarget();
		}
	}
	if (currState_ == STATE::IDLE && idleTime_ <= (SDL_GetTicks() - lastIdleTime)) {
		currState_ = STATE::PATROLLING;
		target_ = patrol_[currPatrol_];
	}
	//Si el pirata está en patrulla
	if (currState_ == STATE::PATROLLING) {
		move(target_);
		SDL_Rect targetPos = SDL_Rect(
			{ (int)target_.getX() / 2,(int)target_.getY() / 2,(int)scale_.getX() / 2,(int)scale_.getY() / 2 });
		SDL_Rect pos = SDL_Rect(
			{ (int)pos_.getX() / 2,(int)pos_.getY() / 2,(int)scale_.getX() / 2,(int)scale_.getY() / 2 });
		if (SDL_HasIntersection(&pos, &targetPos)) {
			currState_ = STATE::IDLE;
			lastIdleTime = SDL_GetTicks();
			if (currPatrol_ == patrol_.size() - 1) {
				currPatrol_ = 0;
			}
			else
			{
				currPatrol_++;
			}
		}
	}
	return false;
}


void EnemyPirate::move(Vector2D posToReach) {
	//establecemos el objetivo para poder parar al llegar
	target_.setVec(posToReach);
	dir_.setX(posToReach.getX() - getCenter().getX());
	dir_.setY(posToReach.getY() - getCenter().getY());
	dir_.normalize();
	double delta = app_->getDeltaTime();
	pos_.setX(pos_.getX() + (dir_.getX() * (currStats_.moveSpeed_ * delta)));
	pos_.setY(pos_.getY() + (dir_.getY() * (currStats_.moveSpeed_ * delta)));
}

//Devuelve true si el enemigo que tengo está a rango y en función de ello cambia entre melee y range
bool EnemyPirate::onRange() {
	if (currEnemy_ == nullptr) {
		return false;
	}
	SDL_Rect enemyRect = SDL_Rect({ (int)currEnemy_->getPosX(),(int)currEnemy_->getPosY(),
		(int)currEnemy_->getScaleX(),(int)currEnemy_->getScaleY() });

	double meleePosX = getPosX() + getScaleX() / 2 - currStats_.meleeRange_ - getScaleX() / 2;
	double meleePosY = getPosY() + getScaleY() / 2 - currStats_.meleeRange_ - getScaleY() / 2;
	double meleeRangeX = currStats_.meleeRange_ + getScaleX() / 2;
	double meleeRangeY = currStats_.meleeRange_ + getScaleY() / 2;

	SDL_Rect meleeAttack = { meleePosX   ,meleePosY,meleeRangeX * 2, meleeRangeY * 2 };
	if (currEnemy_ != nullptr && SDL_HasIntersection(&enemyRect, &meleeAttack)) {
		currAtackStatus_ = ATK_STATUS::MELEE;
		setTexture(app_->getTextureManager()->getTexture(Resources::MonkeyFront));
		return true;
	}
	else if (currEnemy_ != nullptr) {
		double rangePosX = getPosX() + getScaleX() / 2 - currStats_.distRange_ - getScaleX() / 2;
		double rangePosY = getPosY() + getScaleY() / 2 - currStats_.distRange_ - getScaleY() / 2;
		double rangeAttackX = getScaleX() / 2 + currStats_.distRange_;
		double rangeAttackY = getScaleY() / 2 + currStats_.distRange_;

		SDL_Rect rangeAttack = { rangePosX   ,rangePosY,rangeAttackX * 2, rangeAttackY * 2 };
		if (SDL_HasIntersection(&enemyRect, &rangeAttack)) {
			currAtackStatus_ = ATK_STATUS::RANGE;
			setTexture(app_->getTextureManager()->getTexture(Resources::PlayerFront));
			return true;
		}
	}
	else
	{
		false;
	}
}

//Inicializa todas las animaciones
void EnemyPirate::initAnims()
{
	//Para la animación de ataque
	attackAnim_ = Anim(NUM_FRAMES_ATK, W_FRAME_ATK, H_FRAME_ATK, FRAME_RATE_ATK, false);
	//Para la animación de caminar
	walkAnim_ = Anim(NUM_FRAMES_MOV, W_FRAME_MOV, H_FRAME_MOV, FRAME_RATE_MOV, true);
	//Para la animación de parado
	idleAnim_ = Anim(NUM_FRAMES_IDLE, W_FRAME_IDLE, H_FRAME_IDLE, FRAME_RATE_IDLE, true);
}

//Se encarga de gestionar el ataque a melee o distancia DONE
void EnemyPirate::attack() {
	if (currAtackStatus_ == ATK_STATUS::RANGE && currStats_.distRate_ <= SDL_GetTicks() - lastRangeHit_) {
		lastRangeHit_ = SDL_GetTicks();
		Bullet* bullet = new Bullet(app_, app_->getTextureManager()->getTexture(Resources::Bullet),
			getCenter(), currEnemy_->getCenter(), currStats_.distDmg_);
		app_->getCurrState()->addRenderUpdateLists(bullet);
		CollisionCtrl::instance()->addEnemyBullet(bullet);
	}
	else if (currStats_.meleeRate_ <= SDL_GetTicks() - lastMeleeHit_)
	{
		lastMeleeHit_ = SDL_GetTicks();
		auto dmg = dynamic_cast<Player*>(currEnemy_);
		if (dmg != nullptr) {
			dmg->receiveDamage(currStats_.meleeDmg_);
		}
	}

}

//Inicializa al al pirata
void EnemyPirate::initObject() {
	Enemy::initObject();
	initRewards();
	setTexture(app_->getTextureManager()->getTexture(Resources::PlayerFront));
	rangeVision_ = VIS_RANGE;
}

//Esto es un apaño, se eliminara cuando este completa la gestión de muertes
void EnemyPirate::onCollider()
{

}

//Cuando el pirata pierde el agro, se le asigna el agro del player
//Esto lo llama el clon cuando se destruye
void EnemyPirate::lostAggro()
{
	currEnemy_ = GameManager::instance()->getPlayer();
}

//Devuelve true si encontro un enemigo cerca y lo asigna a currEnemy_ DONE
bool EnemyPirate::getEnemy() {
	if (Enemy::getEnemy(rangeVision_)) {
		app_->getAudioManager()->playChannel(Resources::AudioId::Agro, 0, Resources::EnemyPirateChannel);
		
		return true;
	}
	else return false;
}

//Cuando el pirata pierde el agro, se le asigna el agro del player
//Esto lo llama el clon cuando se destruye
void EnemyPirate::lostAgro()
{
	currEnemy_ = GameManager::instance()->getPlayer();
}


//Genera la posici�n a la que se mueve el pirata en funci�n de su rango 
void EnemyPirate::selectTarget() {

	Point2D centerPos = { getPosX() + getScaleX() / 2, getPosY() + getScaleY() / 2 };
	Point2D enemycenterPos = { currEnemy_->getPosX() + currEnemy_->getScaleX() / 2, currEnemy_->getPosY() + currEnemy_->getScaleY() / 2 };
	Vector2D posToReach;
	if (currAtackStatus_ == ATK_STATUS::RANGE) {

		posToReach.setX((enemycenterPos.getX() + currStats_.distRange_) - centerPos.getX());
		posToReach.setY((enemycenterPos.getY() + currStats_.distRange_) - centerPos.getY());
	}
	else
	{
		posToReach.setX((enemycenterPos.getX() + currStats_.meleeRange_) - centerPos.getX());
		posToReach.setY((enemycenterPos.getY() + currStats_.meleeRange_) - centerPos.getY());
	}
	target_ = posToReach;
	move(enemycenterPos);
}
void EnemyPirate::initialStats()
{
	HEALTH = 1000;
	MANA = 100;
	MANA_REG = 100;
	ARMOR = 10;
	MELEE_DMG = 0;
	DIST_DMG = 0;
	CRIT = 2000;
	MELEE_RANGE = 50;
	DIST_RANGE = 75;
	MOVE_SPEED = 250;
	MELEE_RATE = 1500;
	DIST_RATE = 1500;
	initStats(HEALTH,MANA, MANA_REG,ARMOR,MELEE_DMG,DIST_DMG,CRIT,MELEE_RANGE,DIST_RANGE,MOVE_SPEED,MELEE_RATE,DIST_RATE);
}

void EnemyPirate::initRewards()
{
	minGold = 30;
	maxGold = 50;
	minArchievementPoints = 2;
	maxArchievementPoints = 10;
	goldPoints_ = app_->getRandom()->nextInt(minGold, maxGold + 1);
	achievementPoints_ = app_->getRandom()->nextInt(minArchievementPoints, maxArchievementPoints + 1);
}