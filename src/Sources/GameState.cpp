#include "GameState.h"
GameState::~GameState() {
	//Borra todos los objetos de la lista de gameObjects
	for (auto it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
		delete (*it);
	}
	gameObjects_.clear(); //Clear lista objetos
	objectsToRender_.clear(); //Clear lista de objetos a dibujar
}

void GameState::update() {
	//Update de todos los objetos. En los botones que cambian estado devuelve true para dejar de updatear los objetos
	//de ese estado
	for (auto it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
		if((*it)->update())return;
	}

	//Se eliminan los objetos que se han a�adido a la lista de objectsToErase
	for (auto it = objectsToErase_.begin(); it != objectsToErase_.end(); ++it) {
		gameObjects_.remove(*it);

		//Si hereda de Draw tambien se quita de la lista de renders
		if (dynamic_cast<Draw*>(*it) != nullptr)
			objectsToRender_.remove(static_cast<Draw*>(*it));
	}
}

void GameState::draw() const {
	for (auto it = objectsToRender_.begin(); it != objectsToRender_.end(); ++it) {
		(*it)->draw();
	}
}
void GameState::handleEvents() {
	eventHandler_->update();
}
void GameState::createButton(Texture* texture, Vector2D pos, Vector2D scale, CallBackOnClick* callBack, Application* app) {
	Button* button = new Button(app,texture, pos, scale, callBack); //Crea boton
	addRenderUpdateLists(button); //Lo mete en las listas de objetos y de dibujado
}

void GameState::addUpdateList(GameObject* obj) {
	gameObjects_.push_back(obj);
}

void GameState::addRenderList(Draw* obj) {
	objectsToRender_.push_back(obj);
}

void GameState::addRenderUpdateLists(Draw* obj) {
	addUpdateList(obj);
	addRenderList(obj);
}

void GameState::addObjectsToErase(GameObject* obj) {
	objectsToErase_.push_back(obj);
}