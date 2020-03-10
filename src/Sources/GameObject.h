#pragma once
#include "Application.h"
#include "Vector2D.h"
#include "SDL.h"
#include "checkML.h"

using namespace std;
using uint = unsigned int;

class GameObject
{
protected:
	Vector2D pos_, scale_;
	Application* app_ = nullptr;

protected:
	//constructor vac�o
	GameObject() : pos_({ 0,0 }), scale_(0, 0) {};
	//constructor con par�metros
	GameObject(Application* app, Vector2D pos, Vector2D scale) : app_(app), pos_(pos), scale_(scale) {};

	//constructor por cop�a
	GameObject(const GameObject& other) : pos_(other.pos_), scale_(other.scale_), app_(other.app_) {};
	//constructor por movimiento
	GameObject(const GameObject&& other)noexcept : pos_(other.pos_), scale_(other.scale_), app_(other.app_) {};

public:
	virtual bool update() = 0;
	//get
	inline const Vector2D& getPos() const { return pos_; }
	inline const Vector2D& getScale()const { return scale_; }
	inline const double getPosX() { return pos_.getX(); }
	inline const double getPosY() { return pos_.getY(); }
	inline const double getScaleX() { return scale_.getX(); }
	inline const double getScaleY() { return scale_.getY(); }

	//set
	inline void setPosX(double x) { pos_.setX(x); }
	inline void setPosY(double y) { pos_.setY(y); }
	inline void setPos(Vector2D newPos) { pos_.setVec(newPos); }
	inline void setScaleX(double x) { scale_.setX(x); }
	inline void setScaleY(double y) { scale_.setY(y); }
	inline void setScale(Vector2D newPos) { scale_.setVec(newPos); }
};

