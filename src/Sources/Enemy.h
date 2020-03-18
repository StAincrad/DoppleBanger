#pragma once
#include "Actor.h"
using namespace std;
class Enemy : public Actor
{
public:
	//<summary>Constructor tanto por defecto como por contenido si no se le pasan valores ser�n los puestos, si se le pasan valores los editara</summary>
	Enemy(Application* app = nullptr, Texture* texture = nullptr, Vector2D pos = { 0,0 }, Vector2D scale = { 0,0 }, SDL_Rect collisionArea = { 0,0,0,0 }, Stats stats = {}, SDL_Rect frame = { 0,0,0,0 }, int numberFrames = 0) :Actor(app, texture, pos, scale, collisionArea, frame, numberFrames) { currStats_ = stats; };
	//<summary>Constructor por copia</summary>
	Enemy(Enemy& other) :Actor(other.app_, other.texture_, other.pos_, other.scale_, other.collisionArea_) { currStats_ = other.currStats_; };
	//<summary>Constructor por movimiento<summary>
	Enemy(Enemy&& other)noexcept :Actor(other.app_, other.texture_, other.pos_, other.scale_, other.collisionArea_) { currStats_ = other.currStats_; };
	//<summary>Destructor</summary>
	virtual ~Enemy() {};
	//<summary>Metodo que llevar� la logica de nuestros enemigos cada enemigo tiene un comportamiento diferente por los que lo unico com�n es el cambio del frame de la animaci�n </summary>
	virtual bool update() { updateFrame();return false; };
	virtual void onCollider() {};
	//<summary>Metodo que renderizar� nuestro enemigos, (se ha hecho suponiendo que todos tienen animaci�n </summary>
	virtual void draw() const { texture_->render(destiny_, frame_); };
};

