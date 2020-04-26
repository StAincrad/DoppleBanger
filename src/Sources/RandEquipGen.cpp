#include "RandEquipGen.h"
#include "Application.h"
#include "Resources.h"
#include "GameManager.h"
#include "Blunder.h"

RandEquipGen::RandEquipGen(Application* app) : app_(app)
{
	rn_ = app_->getRandom();
	gameManager_ = GameManager::instance(); 
}

Equipment* RandEquipGen::genEquip()
{
	equipType type = static_cast<equipType>(rn_->nextInt(0, 5));
	return genEquip(type);
}

Equipment* RandEquipGen::genEquip(equipType type)
{
	//Aqu� visual me hizo declararlo todo a parte o no compilaba
	int ad, health, armor, price, crit, speed;
	double meleeRate, distRate, distRange;
	
	Equipment* obj;
	int area = (int)gameManager_->getUnlockedIslands();
	switch (type)
	{
	//Vida y armadura
	case Armor_:
		health = rn_->nextInt(7 * area, 16 * area);
		armor = rn_->nextInt(3 * area, 8 * area);
		price = rn_->nextInt(2 * area, 4 * area);
		obj = new Armor(app_->getTextureManager()->getTexture(Resources::TextureId::Wheel), "Pechera", "helloWorld", price, health, armor);
		break;

	//Velocidad y armadura
	case Boots_:
		speed = rn_->nextInt(1 * area, 4 * area);
		armor = rn_->nextInt(2 * area, 6 * area);
		price = rn_->nextInt(1 * area, 2 * area);
		obj = new Boots(app_->getTextureManager()->getTexture(Resources::TextureId::Wheel), "Botas", "helloWorld", price, speed, armor);
		break;

	//Cr�tico y armadura
	case Gloves_:
		crit = rn_->nextInt(3 * area, 8 * area);
		armor = rn_->nextInt(1 * area, 3 * area);
		price = rn_->nextInt(1 * area, 4 * area);
		obj = new Gloves(app_->getTextureManager()->getTexture(Resources::TextureId::Wheel), "Guantes", "helloWorld", price, crit, armor);
		break;

	//Ad y velocidad de ataque
	//Mas alcance que el sable
	case Sword_:
		ad = rn_->nextInt(5 * area, 16 * area);
		meleeRate = rn_->nextInt(3 * area, 6 * area);
		price = rn_->nextInt(3 * area, 5 * area);
		obj = new Sword(app_->getTextureManager()->getTexture(Resources::TextureId::Wheel), "Espada", "helloWorld", price, ad, meleeRate, type);
		break;

	//Mas da�o que la espada
	case Saber_:
		ad = rn_->nextInt(6 * area, 17 * area);
		meleeRate = rn_->nextInt(1 * area, 4 * area);
		price = rn_->nextInt(3 * area, 5 * area);
		obj = new Sword(app_->getTextureManager()->getTexture(Resources::TextureId::Wheel), "Sable", "helloWorld", price, ad, meleeRate, type);
		break;

	//Ap y velocidad de disparo
	//Dispara en linea recta con mas da�o y alcance que la escopeta
	case Pistol_:
		ad = rn_->nextInt(5 * area, 16 * area);
		distRate = rn_->nextInt(2 * area, 4 * area);
		price = rn_->nextInt(4 * area, 6 * area);
		crit = rn_->nextInt(1 * area, 3 * area);
		distRange = rn_->nextInt(2 * area, 6 * area);
		obj = new Gun(app_->getTextureManager()->getTexture(Resources::TextureId::Wheel), "Pistola", "helloWorld", price, ad, distRate, distRange, crit, type, PISTOL_BULLET_SPEED);
		break;

	//Dispara en are recta con menos da�o y alcance que la escopeta
	case Shotgun_:
		ad = rn_->nextInt(3 * area, 13 * area);
		distRate = rn_->nextInt(1 * area, 2 * area);
		price = rn_->nextInt(4 * area, 6 * area);
		crit = rn_->nextInt(2 * area, 6 * area);
		distRange = rn_->nextInt(1 * area, 3 * area);
		distRange = distRange / 2;
		obj = new Gun(app_->getTextureManager()->getTexture(Resources::TextureId::Wheel), "Pistola", "helloWorld", price, ad, distRate, distRange, crit, type, PISTOL_BULLET_SPEED);
		break;
	case Blunderbuss_:
		ad = rn_->nextInt(3 * area, 13 * area);
		distRate = rn_->nextInt(4 * area, 6 * area);
		price = rn_->nextInt(4 * area, 6 * area);
		crit = rn_->nextInt(2 * area, 6 * area);
		distRange = rn_->nextInt(1 * area, 2 * area);
		distRange = distRange / 3;
		obj = new Blunder(app_->getTextureManager()->getTexture(Resources::TextureId::Blunderbuss), "Trabuco", "Trabucón para el más perrón", price, ad, distRate, crit, distRange, type, BLUNDERBUSS_BULLET_SPEED);
		break;
	default:
		return nullptr;
		break;
	}

	return obj;
}