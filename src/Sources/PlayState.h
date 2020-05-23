﻿#pragma once
#include "GameState.h"
#include "CollisionCtrl.h"
#include <list>
#include "TiledMap.h"
#include "AStar.hpp"

class CollisionCtrl;
class HUD;
class Player;
class Enemy;
class Obstacle;

class PlayState : public GameState
{
public:
	///<summary>Constructora PlayState</summary>
	PlayState(Application* app = nullptr) : GameState(app) { initState(); };
	///<summary>Destructora</summary>
	virtual ~PlayState() { delete mousePointer; delete generator_; };

	//Método necesario para dibujar los textbox por encima del juego
	virtual void draw()const;
	///<summary>Llama al update del CollisionManager</summary>
	virtual void update();

	///<summary>Actualizamos le puntero del ratón</summary>
	void updateMousePointer();
	///<summary>Método que settea el player</summary>
	void setPlayer(Player* player) { player_ = player; }

	///<summary>A�ade un objeto a la lista enemies_, objectsToRender y gameObjects_</summary>
	void addEnemy(Enemy* obj);
	///<summary>A�ade un objeto a la lista objects_ (objetos que quieran cambiar el puntero por la mano)</summary>
	void addObject(Collider* obj);
	///<summary>Quita un objeto a la lista enemies_, objectsToRender y gameObjects_</summary>
	void removeEnemy(Enemy* obj);
	///<summary>Quita un objeto a la lista objects_</summary>
	void removeObject(Collider* obj);
	///<summary>Comprueba las posibles acciones del jugador</summary>
	void checkInput();
	///<summary>Encuentra al enemigo más cercano a una posición</summary>
	Enemy* findClosestEnemy(Point2D pos);
	///<summary>Comprueba colisiones con los enemigos y devuelve el primer enemigo en caso de haber colisión</summary>
	Enemy* collidesWithEnemy(Point2D pos, Vector2D scale) {};

	AStar::Generator* getGenerator() { return generator_; };

	//Devuelve la lista de enemigos
	list<Enemy*>& getListEnemies() { return enemies_; };
	//Devuelve el primer enemigo en función de un tag
	Enemy* getEnemyByTag(string tag);
	//intercambia posiciones de dos objetos en el orden de renderizado
	void swapRenders(GameObject* obj, GameObject* other);
	///<summary>Método a redefinir en las clases de los niveles para gestionar el cambio de zona</summary>
	virtual void changeZone() {};
	///<summary>Método para cambiar de zona y borrar todos los objetos menos el HUD (último objeto en la lista objectsToRender)</summary>
	void deleteExceptHUD(Zone newZone);
protected:
	Point2D playerEntry_ = Vector2D(0, 0);
	//Singleton de colisiones
	CollisionCtrl* collisionCtrl_ = nullptr;
	Player* player_ = nullptr;
	HUD* hud_ = nullptr;
	list<Obstacle*> obstacles_;
	list<Enemy*> enemies_;
	list<Collider*> objects_;	//Para cambiar el puntero a la mano

	//Para el puntero del ratón dentro del juego
	SDL_Point point;
	Draw* mousePointer;
	const int W_MOUSE_POINTER = 32;
	const int H_MOUSE_POINTER = 32;

	AStar::Generator* generator_;

	virtual void initState();
	//Resetea la partida de acuerdo al GDD
	void resetGame();
	//Sirve para cargar el estado
	virtual void loadState() {};
};