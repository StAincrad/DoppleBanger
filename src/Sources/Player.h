#pragma once
#include "Actor.h"
#include "HandleEvents.h"
#include "Clon.h"

class Player : public Actor
{
private:
	bool attacking = false;

	Actor* objective_ = nullptr;
	Clon* clon_ = nullptr;
	HandleEvents* eventHandler_ = nullptr;

//<summary>Variables relativas a las habilidades</summary>
#pragma region abilities
	int liberation_ = 2;	//Nivel de la habilidad del clon
	bool explotion_ = false;	//Si tiene la habilidad activada
#pragma endregion

//<summary>Variables de los cooldowns del jugador</summary>
#pragma region cooldowns
	double clonCooldown_ = 5;
	double clonTime_ = 0; //Momento del último clon
	double meleeTime_ = 0; //Momento del último ataque
	double shotTime_ = 0; //Momento del �ltimo disparo
#pragma endregion

//<summary>Estadisticas del jugador</summary>
#pragma region consts
	const double HEALTH = 100;
	const double MANA = 100;
	const double MANA_REG = 1;
	const double ARMOR = 10;
	const double AD = 20;
	const double AP = 0;
	const double CRIT = 0;
	const double RANGE = 50;
	const double MOVE_SPEED = 100;
	const double MELEE_RATE = 1;
	const double DIST_RATE = 2;

	const double CLON_SPAWN_RANGE = 700;
#pragma endregion

	virtual void initObject();

public:
//Constructora de player
	Player(Application* app, Vector2D pos, Vector2D scale) :
		Actor(app, pos, scale) {
		initObject();
	};
	~Player() {};

	};
		currStats_ = other.currStats_;
		eventHandler_ = HandleEvents::instance();
	Player(const Player& other) : Actor(other.app_, other.texture_, other.pos_, other.scale_, other.collisionArea_){
	///<summary>Constructor por copia</summary>
	virtual bool update();
	void shoot(Vector2D dir);
	virtual void onCollider() { /*Colisi�n con enemigo*/ };
	void move(Point2D target);
	void attack(Enemy* obj);
	const int getLiberation() { return liberation_; };
	const bool getExplotion() { return explotion_; };
	const Stats& getStats() { return currStats_; };
};