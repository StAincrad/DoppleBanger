#pragma once
#include <memory>
#include <list>
#include <vector>
#include <iostream>
#include "InventoryButton.h"
#include "RandEquipGen.h"
#include "Equipment.h"
#include "Clon.h"
#include "HUD.h"
#include "checkML.h"

class Application;

using namespace std;
class Item;
class Player;
using lista = list<InventoryButton*>*;

//Enumerados que representan la �ltima isla desbloqueada
enum class Zone : int {
	CaribeanA = 1,
	CaribeanB = 2,
	CaribeanC = 3,
	CaribeanBoss = 4,
	SpookyA = 5,
	SpookyB = 6,
	SpookyC = 7,
	SpookyD = 8,
	SpookyBoos = 9,
	Volcanic = 10
};
enum class Island : int {
	Caribbean,
	Spooky,
	Volcanic
};
enum class spentPoints : int {
	Precision = 0,
	Melee = 0,
	Ghost = 0
};

enum class missions : int {
	gallegaEnProblemas = 0,
	papelesSiniestros,
	masValePajaroEnMano,
	arlongPark
	//En caso de tener misión para el loro añadirla tb (o si se añade el esqueleto)
};

#pragma region Skills

///Tipos de Skills que puede haber
enum class SkillType { Active, Pasive };

//Nombre de cada una de las habilidades del jugador
enum class SkillName : int {
	//Enums auxiliares para las pasivas y para las habilidades que no están equipadas
	Unequipped,

	//Ataque melee
	GolpeFuerte,
	Invencible,
	Torbellino,

	//Ataque a distancia
	DisparoPerforante,
	Raudo,
	Rebote,

	//Clon
	Clon,
	LiberacionI,
	Explosion,
	LiberacionII,
};

//Enum para identificar las teclas de las habilidades
enum class Key : int {
	Q,
	W,
	E,
	R,
	One,
	Two
};
#pragma endregion
#pragma region Objetos
enum class ObjectName : int {
	Unequipped,

	//Pociones
	Health,
	Mana,
	Speed,
	Armor,
	Dmg,
	Crit
};
#pragma endregion

class GameManager {
private:
	//Numero de misiones secundarias
	static const int NUM_MISION = 10;
	//Puntero unico para evitar copias
	static unique_ptr<GameManager> instance_;
	//Puntos de haza�a
	int achievementPoints_ = 350;
	//Cantidad de dinero almacenada en el inventario
	int inventoryGold_ = 300;
	//Cantidad de dinero almacenada en el alijo
	int stashGold_ = 1000;
	//Booleano que indica si estamos en el barco
	bool onShip_ = true;
	//Enum de la �ltima isla desbloqueada
	Island unlockedIslands_ = Island::Caribbean;
	//Enum de la isla actual
	Island currIsland_ = Island::Caribbean;
	//Puntos de haza�a gastados en la rama precisi�n
	spentPoints precision_ = spentPoints::Precision;
	//Puntos de haza�a gastados e la rama melee
	spentPoints melee_ = spentPoints::Melee;
	//Puntos de haza�a gastados de la rama ghost
	spentPoints ghost_ = spentPoints::Ghost;
	//Puntero a la lista de item del inventario
	list<InventoryButton*>* inventory_ = new list<InventoryButton*>;
	//Puntero a la lista de items del alijo
	list<InventoryButton*>* stash_ = new list<InventoryButton*>;
	//Vector que representa las misiones secundarias completadas
	vector<bool> missionsComplete_ = vector<bool>(NUM_MISION);
	//Vector que representa las misiones secundarias empezadas
	vector<bool> missionsStarted_ = vector<bool>(NUM_MISION);
	//Vector de cooldowns de las habilidades equipadas
	vector<bool> skillsCooldown_ = { false, false, false, false };
	//Vector que contiene las habilidades desbloquedadas v[Skillname] corresponde con si está desbloqueda
	vector<bool> skillsUnlocked_ = { false, false ,false, false, false, false, false, true, false, false, false }; //Clon inicializada por defecto
	//Vector que contiene la cantidad de enemigos que se han muerto de la mision correspondiente
	//<gallegaEnProblemas, papelesSiniestros, masValePajaroEnMano, arlongPark >
	vector<int> countEnemiesMission_ = { 0, 0, 0, 0};
	//Vector que contiene el numero de enemigos que se tiene que matar en cada mision
	vector<int> enemiesMission_ = { 3, 2, 2, 4 };
	//Vector que contiene las habilidades equipadas
	vector<SkillName> skillsEquipped_ = { SkillName::Unequipped, SkillName::Unequipped, SkillName::Unequipped, SkillName::Clon };
	//Vector que contiene los objetos equipados
	vector<ObjectName> objectsEquipped = { ObjectName::Unequipped, ObjectName::Unequipped };

	//Puntero al player a falta de estipular las variables que van a ir en gameManager sobre el player
	Player* player_ = nullptr;
	//Puntero al clon
	GameObject* clon_ = nullptr;
	//Puntero al HUD
	HUD* hud_ = nullptr;
	//puntero a la aplicacion
	Application* app_ = nullptr;
public:
	//Constructor vacio
	GameManager() {
		unlockedIslands_ = Island::Volcanic;
		for (int i = 0; i < NUM_MISION; i++) {
			missionsComplete_[i] = false;
		}
	}
	//Destructor
	~GameManager() {
		for (InventoryButton* ob : *inventory_)delete ob;
		for (InventoryButton* ob : *stash_)delete ob;
		delete inventory_;
		delete stash_;
	}
	//Construye un nuevo gameManger si es null
	static GameManager* instance() {
		if (instance_.get() == nullptr) {
			instance_.reset(new GameManager());
		}
		return instance_.get();
	}
	GameManager(GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	//Inicializa el oro, la actual isla y los puntos de haza�a
	inline void initGameManager(int currGold, Island unlockedIslands, int achievementPoints);

#pragma region getters
	//Devuelve true si la misi�n ha sido pasada
	const bool isThatMissionPass(missions mission) { return missionsComplete_[(int)mission]; };
	//Devuelve true si la misi�n est� empezada
	const bool isThatMissionStarted(missions mission) { return missionsStarted_[(int)mission]; };
	//Devuelve si está activo el cooldown de la habilidad
	const bool getSkillCooldown(int skill) { return skillsCooldown_[skill]; };
	//Devuelve si la skill está desbloqueda
	const bool isSkillUnlocked(SkillName skill) { return skillsUnlocked_[(int)skill]; };
	//Devuelve si la skill está equipada
	const bool isSkillAsign(SkillName skill);
	//Devuelve si estamos o no en la isla
	const bool getOnShip() { return onShip_; };

	//Devuelve el oro conseguido
	const int getGold() { return inventoryGold_; };
	//Devuelve los puntos de haza�a
	const int getAchievementPoints() { return achievementPoints_; };
	//Devuelve el dinero del inventario
	const int getInventoryGold() { return inventoryGold_; }
	//Devuelve el dinero del alijo
	const int getStashGold() { return stashGold_; }
		//Devuelve el tamaño de fuente según el tamaño de la ventana
	const int getFontSize();
	//Devuelve el total de misiones secundarias
	const int getNumMission() { return NUM_MISION; };
	//Devuelve el numero de enemigos muertos en cada mision
	const int getCounterEnemiesMission(missions tagMission) { return countEnemiesMission_.at((int)tagMission); };
	//Devuelve el numero de enemigos que hay que matar en cada mision
	const int getEnemiesMission(missions tagMission) { return enemiesMission_.at((int)tagMission); };
	//Devuelve el inventario
	const lista getInventory() { return inventory_; };
	//Devuelve el alijo
	const lista getStash() { return stash_; };
	//Devuelve el n�mero de islas desbloqueadas
	const Island getUnlockedIslands() { return unlockedIslands_; };
	//Devuelve la isla actual
	const Island getCurrIsland() { return currIsland_; }
	//Devuelve el vector de skills
	const vector<SkillName>& getAllSkillsEquipped() { return skillsEquipped_; }

	//Devuelve los puntos gastados en la rama presici�n
	const spentPoints getPresicionPoints() { return precision_; };
	//Devuelve los puntos gastados en la rama melee
	const spentPoints getMeleePoints() { return melee_; };
	//Devuelve los puntos gastados en la rama fantasma
	const spentPoints getGhostPoints() { return ghost_; };
	
	//Devuele la habilidad asignada en la tecla
	const SkillName getEquippedSkill(Key key) { return skillsEquipped_[(int)key]; }
	//Devuelve la tecla en la que está equipada la habilidad
	const Key getEquippedSkillKey(SkillName skill);
	//Devuelve el objeto equipado
	const ObjectName getObjectEquipped(Key key) { return objectsEquipped[(int)key - (int)Key::One]; };
	
	//Devuelve la posici�n del player
	const Point2D getPlayerPos();
	//Devuelve al jugador
	Player* getPlayer() { return player_; };
	//Devuelve al clon
	GameObject* getClon() { return clon_; };
	
#pragma endregion

#pragma region setters
	//Suma puntos de hazaña
	inline void addArchievementPoints(int value) { achievementPoints_ += value; }
	//Asigna los archievement points
	inline void setArchievementPoints(int value) { achievementPoints_ = value; };
	//Asigna los puntos gastados a la rama precision
	inline void setPrecisionPoints(int value) { precision_ = (spentPoints)value; };
	//Asigna los puntos gastados a la rama melee
	inline void setMeleePoints(int value) { melee_ = (spentPoints)value; };
	//Asigna los puntos gastados a la rama Ghost
	inline void setGhostPoints(int value) { ghost_ = (spentPoints)value; };
	//Añade(+) /Quita(-) dinero del inventario
	inline void addInventoryGold(int money) { inventoryGold_ += money; };
	////Añade(+) /Quita(-) dinero del alijo
	inline void addStashGold(int money) { stashGold_ += money; };
	///Asigna money como cantidad de dinero en el inventario
	inline void setInventoryGold(int money) { inventoryGold_ = money; };
	///Asigna money como cantidad de dinero en el alijo
	inline void setStashGold(int money) { stashGold_ = money; };
	//Indica si estamos o no en el barco
	inline void setOnShip(bool onShip) { onShip_ = onShip; };

	//Reseteo del inventario
	void resetInventory();
	//Asigna el puntero al inventario
	inline void setInventoryPointer(lista inv) { inventory_ = inv; };
	//Asigna el puntero al alijo
	inline void setStashPointer(lista stash) { stash_ = stash; };
	
	//Añade una muerte al contador de muertes de la mision
	inline void addMissionCounter(missions tagMission) { countEnemiesMission_.at((int)tagMission)++; }
	//Reinicia el contador de muertes de la mision
	inline void resetMissionCounter(missions tagMission) { countEnemiesMission_.at((int)tagMission) = 0; }
	//Completa una misi�n secundaria
	inline void setCompleteMission(missions mission, bool complete) { missionsComplete_[(int)mission] = complete; };
	//Empieza una misi�n secundaria
	inline void setStartedMission(missions mission, bool started) { missionsStarted_[(int)mission] = started; };
	//Asigna a la ultima isla desbloqueada
	inline void setUnlockedIslands(Island island) { unlockedIslands_ = island; };
	//Asigna la nueva isla actual
	inline void setCurrIsland(Island newIsland) { currIsland_ = newIsland; };
	//Marca como desbloqueda la skill que pases como parámetro
	inline void setSkillUnlocked(SkillName skill) { skillsUnlocked_[(int)skill] = true; };

	//Actualiza el estado del cooldown, no es inline por el HUD
	void setSkillCooldown(bool cooldown, Key key);
	//Actualiza la habilidad equipada en el HUD y en el vector, no es inline por el HUD
	void setSkillEquiped(SkillName newSkill, Key key);
	//Actualiza el objeto equipado en el HUD y en el vector, no es inline por el HUD
	void setObjectEquipped(ObjectName newObject, Key key);

	//Asigna al puntero de player
	inline void setPlayer(Player* player) { player_ = player; };
	//Asigna al puntero de clon
	inline void setClon(GameObject* clon) { clon_ = clon; };
	//Asigna el puntero de hud
	inline void setHUD(HUD* hud) { hud_ = hud; };

	//borra al clon
	inline void deleteClon() { clon_ = nullptr; };
	//aplicacion
	inline void setApp(Application* app) { app_ = app; };
#pragma endregion
	//Para añadir objetos al inventario
	void addToInventory(Equipment* ob);
};