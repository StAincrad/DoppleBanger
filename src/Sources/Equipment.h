#pragma once
#include"checkML.h"

#include "Item.h"
#include "Stats.h"
#include <iostream>





enum class equipType { Armor, Gloves, Boots, Sword, Saber, Pistol, Shotgun };

class Equipment : public Item
{
protected:
	equipType type_;
	Stats modifiers_;

public:
	//Crea un item equipable del tipo dado
	Equipment(Texture* texture, string name, string desc, double price, equipType type) : 
		Item(texture, name, desc, price, 1), type_(type) {};
	virtual ~Equipment() {}; //Destructora generica

#pragma region Setters
	void setHealth(double health) { modifiers_.health_ = health; };// Asigna el valor de Health
	void setArmor(double armor) { modifiers_.armor_ = armor; };// Asigna el valor de Armor
	void setMeleeDmg(double ad) { modifiers_.meleeDmg_ = ad; }; // Asigna el valor de AD
	void setDistDmg(double distDmg) { modifiers_.distDmg_ = distDmg; }; // Asigna el valor de distDmg
	void setCrit(double crit) { modifiers_.crit_ = crit; }; // Asigna el valor de Crit
	void setSpeed(double speed) { modifiers_.moveSpeed_ = speed; }; // Asigna el valor de Speed
	void setDistRate(double distRate) { modifiers_.distRate_ = distRate; }; // Asigna el valor de DistRate
	void setMeleeRate(double meleeRate) { modifiers_.meleeRate_ = meleeRate; }; // Asigna el valor de MeleeRate
#pragma endregion
	

#pragma region Getters
	double getHealth() { return modifiers_.health_; }; // Devuelve el valor de Health
	double getArmor() { return modifiers_.armor_; }; // Devuelve el valor de Armor
	double getMeleeDmg() { return modifiers_.meleeDmg_; }; // Devuelve el valor de AD
	double getDistDmg() { return modifiers_.distDmg_; }; // Devuelve el valor de DistDamage
	double getCrit() { return modifiers_.crit_; }; // Devuelve el valor de Crit
	double getSpeed() { return modifiers_.moveSpeed_; }; // Devuelve el valor de Speed
	double getDistRate() { return modifiers_.distRate_; }; // Devuelve el valor de DistRate
	double getMeleeRate() { return modifiers_.meleeRate_; }; // Devuelve el valor de MeleeRate
	equipType getEquipTye() { return type_; };	//Devuelve el tipo del objeto
#pragma endregion
	virtual void equip(Player* player) = 0;
	virtual void remove(Player* player) = 0;
	virtual void writeStats() = 0;
};