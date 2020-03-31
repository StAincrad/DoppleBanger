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
	for (auto it = objectsToRemove_.begin(); it != objectsToRemove_.end(); ++it) {
		gameObjects_.remove(*it);
		delete (*it);
	}
	objectsToRemove_.clear();

	for (auto it = rendersToRemove_.begin(); it != rendersToRemove_.end(); ++it) {
		objectsToRender_.remove(*it);
		delete (*it);
	}
	rendersToRemove_.clear();

	for (auto it = objRendToRemove_.begin(); it != objRendToRemove_.end(); ++it) {
		gameObjects_.remove(*it);
		objectsToRender_.remove(*it);
		delete (*it);
	}
	objRendToRemove_.clear();

	for (auto it = gameObjects_.begin(); it != gameObjects_.end(); ++it) {
		if ((*it)->update())return;
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

void GameState::removeUpdateList(GameObject* obj) {
	objectsToRemove_.push_back(obj);
}

void GameState::removeRenderList(Draw* obj) {
	rendersToRemove_.push_back(obj);
}

void GameState::removeRenderUpdateLists(Draw* obj) {
	objRendToRemove_.push_back(obj);
}