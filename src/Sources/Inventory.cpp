#include "Inventory.h"
#include "Armor.h"
#include "Gloves.h"
#include "Boots.h"
#include "Sword.h"
#include "Gun.h"
#include "usable.h"
#include "Player.h"
#include "SkillState.h"

using namespace std;

//callbacks
void callSelectObject(Application* app, InventoryButton* but) {
	dynamic_cast<Inventory*>(app->getCurrState())->selectObject(but);
}
void callDeleteObject(Application* app) {
	dynamic_cast<Inventory*>(app->getCurrState())->deleteObj();
}
void callEquipedObject(Application* app) {
	dynamic_cast<Inventory*>(app->getCurrState())->equipObject();
}
void callForwardList(Application* app) {
	dynamic_cast<Inventory*>(app->getCurrState())->forwardList();
}
void callBackList(Application* app) {
	dynamic_cast<Inventory*>(app->getCurrState())->backList();
}
void callExit(Application* app){
	SDL_ShowCursor(SDL_DISABLE);
	app->getGameStateMachine()->popState();
}
void callSkillsState(Application* app) {
	app->getGameStateMachine()->changeState(new SkillState(app));
}

void Inventory::initState(){
	//Fondo
	SDL_ShowCursor(SDL_ENABLE);

	background_ = app_->getTextureManager()->getTexture(Resources::InventoryMenu);
	Vector2D auxpos = Vector2D{ 12 * (double)(app_->getWindowWidth() / 14), 14 * (double)(app_->getWindowHeight() / 17) };
	Vector2D auxSize = Vector2D{ (double)(app_->getWindowWidth() / 27),  (double)(app_->getWindowWidth() / 27) };
	deleteButton_ = new Button(app_, app_->getTextureManager()->getTexture(Resources::DeleteButton), auxpos, auxSize, callDeleteObject);
	addRenderUpdateLists(deleteButton_);

	//Boton para equipar
	auxpos = Vector2D{ 12 * (double)(app_->getWindowWidth() / 14), 9 * (double)(app_->getWindowHeight() / 12) };
	auxSize = Vector2D{ (double)(app_->getWindowWidth() / 27),  (double)(app_->getWindowWidth() / 27) };
	equippedButton_ = new Button(app_, app_->getTextureManager()->getTexture(Resources::EquippedButton), auxpos, auxSize, callEquipedObject);
	addRenderUpdateLists(equippedButton_);

	//Boton para iterar en la lista - Derecha
	auxpos = Vector2D{ 10 * (double)(app_->getWindowWidth() / 12), 3 * (double)(app_->getWindowHeight() / 10) };
	auxSize = Vector2D{ (double)(app_->getWindowWidth() / 30),  (double)(app_->getWindowWidth() / 30) };
	advanceButton_ = new Button(app_, app_->getTextureManager()->getTexture(Resources::RightArrow), auxpos, auxSize, callForwardList);
	addRenderUpdateLists(advanceButton_);

	//Boton para iterar en la lista - Izquierda
	auxpos = Vector2D{ 8 * (double)(app_->getWindowWidth() / 13), 3 * (double)(app_->getWindowHeight() / 10) };
	auxSize = Vector2D{ (double)(app_->getWindowWidth() / 30),  (double)(app_->getWindowWidth() / 30) };
	gobackButton_ = new Button(app_, app_->getTextureManager()->getTexture(Resources::LeftArrow), auxpos, auxSize, callBackList);
	addRenderUpdateLists(gobackButton_);

	//Boton para salir del inventario
	auxpos = Vector2D{ 8 * (double)(app_->getWindowWidth() / 9), 2 * (double)(app_->getWindowHeight() / 9) };
	auxSize = Vector2D{ (double)(app_->getWindowWidth() / 25),  (double)(app_->getWindowWidth() / 25) };
	exitButton_ = new Button(app_, app_->getTextureManager()->getTexture(Resources::ButtonX), auxpos, auxSize, callExit);
	addRenderUpdateLists(exitButton_);

	//Boton para ir al SkillState
	auxpos = Vector2D{ 7 * (double)(app_->getWindowWidth() / 13), 1 * (double)(app_->getWindowHeight() / 40) };
	auxSize = Vector2D{ (double)(app_->getWindowWidth() / 2.62),  (double)(app_->getWindowWidth() / 7.5) };
	goToSkillsButton_ = new Button(app_, app_->getTextureManager()->getTexture(Resources::TextureId::Wheel), auxpos, { 610, 120 }, callSkillsState);
	addRenderUpdateLists(goToSkillsButton_);

	//Cogemos los objetos equpados de player
	player_ = gm_->getPlayer();

	playerEquipment aux = gm_->getEquip();

	if (aux.armor_ != nullptr) equipment_.armor_ = new InventoryButton(app_, Vector2D{ 300,400 }, Vector2D{ 50,50 }, aux.armor_, callSelectObject, true);
	if (aux.gloves_ != nullptr) equipment_.gloves_ = new InventoryButton(app_, Vector2D{ 300,400 }, Vector2D{ 50,50 }, aux.gloves_, callSelectObject, true);
	if (aux.boots_ != nullptr) equipment_.boots_ = new InventoryButton(app_, Vector2D{ 300,400 }, Vector2D{ 50,50 }, aux.boots_, callSelectObject, true);
	if (aux.sword_ != nullptr)	equipment_.sword_ = new InventoryButton(app_, Vector2D{ 300,400 }, Vector2D{ 50,50 }, aux.sword_, callSelectObject, true);
	if (aux.gun_ != nullptr) equipment_.gun_ = new InventoryButton(app_, Vector2D{ 300,400 }, Vector2D{ 75,75 }, aux.gun_, callSelectObject, true);
	if (aux.potions_[0] != nullptr)	equipment_.potion1_ = new InventoryButton(app_, Vector2D{ 300,400 }, Vector2D{ 50,50 }, aux.potions_[0], callSelectObject, true);
	if (aux.potions_[1] != nullptr)	equipment_.potion2_ = new InventoryButton(app_, Vector2D{ 300,400 }, Vector2D{ 50,50 }, aux.potions_[1], callSelectObject, true);

	//Cogemos la lista de objetos del gameManager
	inventoryList_ = gm_->getInventory();
	ListPos = inventoryList_->begin();

	///Reasignamos el callback y el estado puesto que si se borra el antiguo inventario, no se podr� seleccionar 
	//ninguno de los objetos al no estar la funci�n en la misma direccion de memoria
	//pasa lo mismo con el estado al que apunta el bot�n
	for (auto ob = inventoryList_->begin(); ob != inventoryList_->end(); ++ob) {
		(*ob)->setNewCallBack(callSelectObject);
	}
}

void Inventory::selectObject(InventoryButton* ob) {
	select_ = ob;
	printInformation();//este metodo todabia no hace nada
}

void Inventory::equipObject() {
	//comprobamos si hay un objeto seleccionado y que no se trate de un objeto ya equipado
	if (select_ != nullptr && !select_->isEquipped()) {
		//comprobamos si se trata de un equipment o un usable
		switch (select_->getObject()->getObjectType())
		{
		case ObjectType::Equipment:
			selectEquipment();
			break;
		case ObjectType::Usable:
			equipPotion();
			break;
		}
	}
}

void Inventory::deleteObj() {
	//Comprobamos si hay algun elemento seleccionado
	if (select_ != nullptr) {
		// comprobamos que no se trate de un elemento equipado
		if (!select_->isEquipped()) {
			if (select_->getIterator() == ListPos)++ListPos;
			inventoryList_->erase(select_->getIterator());
			//Eliminamos el objeto
			delete select_;
			select_ = nullptr;

		}
	}
}

// este metodo desequipa el objeto si esta equipado y equipa el nuevo objeto
void Inventory::changeEquipment(InventoryButton* &but) {
	//Si ya hay un objeto equipado
	if (but != nullptr) {
	//desequipamos el objeto actual
		but->getObject()->remove(player_);
		but->Enable(false);
		InventoryButton* aux = but;
		//El objeto desequipado lo devolvemos al final de la lista
		list <InventoryButton*>::iterator it = inventoryList_->insert(inventoryList_->end(), aux);
		aux->setIterator(it);
	}
	//equipamos el nuevo objeto
	but= select_; 
}

void Inventory::selectEquipment(){
	//comprobamos de que tipo es
	Equipment* auxEquip = static_cast<Equipment*>(select_->getObject());
	equipType auxType = auxEquip->getEquipType();
	//Seleccion del tipo de equipamiento (Pechera, Guantes, Botas, Espada o Pistola)
	if (auxType == equipType::ArmorI || auxType == equipType::ArmorII) {
		changeEquipment(equipment_.armor_);
		player_->equip(static_cast<Armor*>(auxEquip));
	}
	else if (auxType == equipType::GlovesI || auxType == equipType::GlovesII) {
		changeEquipment(equipment_.gloves_);
		player_->equip(static_cast<Gloves*>(auxEquip));
	}
	else if (auxType == equipType::BootsI || auxType == equipType::BootsII) {
		changeEquipment(equipment_.boots_);
		player_->equip(static_cast<Boots*>(auxEquip));
	}
	else if (auxType == equipType::SwordI || auxType == equipType::SwordII
		|| auxType == equipType::SaberI || auxType == equipType::SaberII) {
		changeEquipment(equipment_.sword_);
		player_->equip(static_cast<Sword*>(auxEquip));
	}
	else if (auxType == equipType::PistolI || auxType == equipType::PistolII
		|| auxType == equipType::ShotgunI || auxType == equipType::ShotgunII) {
		changeEquipment(equipment_.gun_);
		player_->equip(static_cast<Gun*>(auxEquip));
	}
	select_->Enable(true);
	auxEquip->equip(player_);
	if (select_->getIterator() == ListPos) ++ListPos;
	inventoryList_->erase(select_->getIterator());
}

void Inventory::equipPotion() {
	//Si ya hay un objeto equipado
	if (slotPotion == 0 && (equipment_.potion1_ == nullptr || equipment_.potion2_ != nullptr)) {
		if (equipment_.potion1_ != nullptr) {
			//Se desequipa el objeto actual
			equipment_.potion1_->Enable(false);
			InventoryButton* aux = equipment_.potion1_;
			//El objeto desequipado lo devolvemos al final de la lista
			list <InventoryButton*>::iterator it = inventoryList_->insert(inventoryList_->end(), aux);
			aux->setIterator(it);
		}
		//Se equipa
		equipment_.potion1_ = select_;
		usable* auxPotion = static_cast<usable*>(select_->getObject());
		player_->equipPotion1(auxPotion);
		//Se pone en el HUD
		gm_->setObjectEquipped((ObjectName)auxPotion->getType(), Key::One);
		//Se elimina de la lista
		select_->Enable(true);
		if (select_->getIterator() == ListPos) ++ListPos;
		inventoryList_->erase(select_->getIterator());

		slotPotion = 1;
	}
	else {
		if (equipment_.potion2_ != nullptr) {
			//Se desequipa el objeto actual
			equipment_.potion2_->Enable(false);
			InventoryButton* aux = equipment_.potion2_;
			//El objeto desequipado lo devolvemos al final de la lista
			list <InventoryButton*>::iterator it = inventoryList_->insert(inventoryList_->end(), aux);
			aux->setIterator(it);
		}
		//Se equipa
		equipment_.potion2_ = select_;
		usable* auxPotion = static_cast<usable*>(select_->getObject());
		player_->equipPotion2(auxPotion);
		//Se pone en el HUD
		gm_->setObjectEquipped((ObjectName)auxPotion->getType(), Key::Two);
		//Se elimina de la lista
		select_->Enable(true);
		if (select_->getIterator() == ListPos) ++ListPos;
		inventoryList_->erase(select_->getIterator());
		slotPotion = 0;
	}
	
}

//boton que avanza en la lista para mostrar los siguientes elementos, en este caso avazara el iterador
void Inventory::forwardList() {
	int aux = advanced;
	aux += 1;
	if (((aux * VIEW_LIST))<= inventoryList_->size()){
		advanced = aux;
		advance(ListPos, VIEW_LIST);//avanzamos el iterador
	}
}

//metodo para retroceder en la lista del inventario
void Inventory::backList() {
	int aux = advanced;
	aux -= 1;
	if ((aux * VIEW_LIST) >= 0) {
		advanced = aux;
		advance(ListPos, -VIEW_LIST);//retrocedemos el iterador
	}
}

void Inventory::draw()const {
	//dibujamos el fondo
	background_->render(SDL_Rect{ 0,0,app_->getWindowWidth(), app_->getWindowHeight() });
	GameState::draw();//dibujamos todos los botones normales
	//Despues dibujaremos SOLO los botones de la lista que salen por pantalla
	if (!inventoryList_->empty()) {
		list<InventoryButton*>::iterator aux = ListPos;
		InventoryButton* auxOb = nullptr;
		// posiciones estandar de los botones
		
		double posx = 9 * (double)(app_->getWindowWidth() / 16);
		double posy = 4 * (double)(app_->getWindowHeight() / 11);
		
		double sizeX = (double)(app_->getWindowWidth() / 16);
		double SizeY = (double)(app_->getWindowWidth() / 16);
		int i = 0;
		//dibujamos los objetos de la primera columna
		while (i < VIEW_LIST / 2 && aux != inventoryList_->end()) {
			auxOb = *aux;
			auxOb->setPos(Vector2D{ double(posx),double(posy + double(i * (double(app_->getWindowHeight()) / 9)) )});
			auxOb->setScale(Vector2D{ sizeX,SizeY });
			auxOb->draw();//desreferenciamos el puntero
			aux++;
			i++;
		}
		//dibujamos los objetos de la segunda colmna
		int j = 0;
		while (j < VIEW_LIST / 2 && aux != inventoryList_->end()) {
			auxOb = *aux;
			auxOb->setPos(Vector2D{ double(posx + (double)(app_->getWindowWidth() / 5)),double(posy + (j * (double(app_->getWindowHeight()) / 9))) });
			auxOb->setScale(Vector2D{ sizeX,SizeY });
			auxOb->draw();//desreferenciamos el puntero
			aux++;
			j++;
		}
	}
	double posx, posy, sizeX, sizeY;
	
	// falta ajustar la posicion de los botones
	//Posicionamiento del equipo actual
	if (equipment_.gloves_ != nullptr) {
		 posx = 2.6 * (double)(app_->getWindowWidth() / 21);
		 posy = 3 * (double)(app_->getWindowHeight() / 10);
		 sizeX = (double)(app_->getWindowWidth() / 12);
		 sizeY = (double)(app_->getWindowWidth() / 12);
		equipment_.gloves_->setPos(Vector2D{ posx,posy });
		equipment_.gloves_->setScale(Vector2D{ sizeX,sizeY });
		equipment_.gloves_->draw();
	}
	if (equipment_.gun_ != nullptr) {
		posx = 2 * (double)(app_->getWindowWidth() / 19);
		posy = 7 * (double)(app_->getWindowHeight() / 13);
		sizeX = (double)(app_->getWindowWidth() / 11);
		sizeY = (double)(app_->getWindowWidth() / 18);
		equipment_.gun_->setPos(Vector2D{ posx,posy });
		equipment_.gun_->setScale(Vector2D{ sizeX,sizeY });
		equipment_.gun_->draw();
	}
	if (equipment_.sword_ != nullptr) {
		posx = 7 * (double)(app_->getWindowWidth() / 20);
		posy = 5 * (double)(app_->getWindowHeight() / 14);
		sizeX = (double)(app_->getWindowWidth() / 20);
		sizeY = (double)(app_->getWindowWidth() / 6);
		equipment_.sword_->setPos(Vector2D{ posx,posy});
		equipment_.sword_->setScale(Vector2D{ sizeX,sizeY });
		equipment_.sword_->draw();
	}
	if (equipment_.armor_ != nullptr) {
		posx = 5 * (double)(app_->getWindowWidth() / 21);
		posy = 5.2 * (double)(app_->getWindowHeight() / 17);
		sizeX = (double)(app_->getWindowWidth() / 13);
		sizeY = (double)(app_->getWindowWidth() / 13);
		equipment_.armor_->setPos(Vector2D{ posx,posy });
		equipment_.armor_->setScale(Vector2D{ sizeX,sizeY });
		equipment_.armor_->draw();
	}
	if (equipment_.boots_ != nullptr) {
		posx = 5 * (double)(app_->getWindowWidth() / 21);
		posy = 9 * (double)(app_->getWindowHeight() / 17);
		sizeX = (double)(app_->getWindowWidth() / 15);
		sizeY = (double)(app_->getWindowWidth() / 15);
		equipment_.boots_->setPos(Vector2D{ posx,posy });
		equipment_.boots_->setScale(Vector2D{ sizeX,sizeY });
		equipment_.boots_->draw();
	}
	if (equipment_.potion1_ != nullptr) {
		posx = 9.3 * (double)(app_->getWindowWidth() / 21);
		posy = 5.5 * (double)(app_->getWindowHeight() / 17);
		sizeX = (double)(app_->getWindowWidth() / 18);
		sizeY = (double)(app_->getWindowWidth() / 18);
		equipment_.potion1_->setPos(Vector2D{ posx,posy });
		equipment_.potion1_->setScale(Vector2D{ sizeX,sizeY });
		equipment_.potion1_->draw();
	}
	if (equipment_.potion2_ != nullptr) {
		posx = 9.3 * (double)(app_->getWindowWidth() / 21);
		posy = 9.3 * (double)(app_->getWindowHeight() / 17);
		sizeX = (double)(app_->getWindowWidth() / 18);
		sizeY = (double)(app_->getWindowWidth() / 18);
		equipment_.potion2_->setPos(Vector2D{ posx,posy });
		equipment_.potion2_->setScale(Vector2D{ sizeX,sizeY });
		equipment_.potion2_->draw();
	}
}

void Inventory::update() {
	if (eventHandler_->isKeyDown(SDLK_c)) {
		callExit(app_);
	}
	else {
		//comprobamos si la lista del inventario no esta vacia, si no lo esta se actualizan los objetos de la lista
		if (!inventoryList_->empty()) {
			list<InventoryButton*>::iterator aux = ListPos;
			InventoryButton* auxOb = nullptr;
			int i = 0;
			//Actualizaremos SOLO los botones de la lista que salen por pantalla
			while (i < VIEW_LIST && aux != inventoryList_->end()) {
				auxOb = *aux;
				auxOb->update();//desreferenciamos el puntero
				aux++;
				i++;
			}
		}
		if (equipment_.gloves_ != nullptr) {
			equipment_.gloves_->update();
		}
		if (equipment_.armor_ != nullptr) {
			equipment_.armor_->update();
		}
		if (equipment_.boots_ != nullptr) {
			equipment_.boots_->update();
		}
		if (equipment_.gun_ != nullptr) {
			equipment_.gun_->update();
		}

		if (equipment_.sword_ != nullptr) {
			equipment_.sword_->update();
		}
		if (equipment_.potion1_ != nullptr) {
			equipment_.potion1_->update();
		}
		if (equipment_.potion2_ != nullptr) {
			equipment_.potion2_->update();
		}

		SDL_Point point;
		point.x = int(eventHandler_->getRealMousePos().getX());
		point.y = int(eventHandler_->getRealMousePos().getY());

		if (SDL_PointInRect(&point, &goToSkillsButton_->getDestiny())) {
			goToSkillsButton_->setTexture(app_->getTextureManager()->getTexture(Resources::TextureId::GoToSkillsBButton));
		}
		else {
			goToSkillsButton_->setTexture(app_->getTextureManager()->getTexture(Resources::TextureId::GoToSkillsAButton));
		}
		//Actualizamos los objetos normales

		GameState::update();
	}
}

Inventory::~Inventory() {
	delete equipment_.armor_;
	delete equipment_.gloves_;
	delete equipment_.gun_;
	delete equipment_.sword_;
	delete equipment_.boots_;
	delete equipment_.potion1_;
	delete equipment_.potion2_;
}
