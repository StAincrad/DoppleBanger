#include "InventoryState.h"

void InventoryState::initState()
{
	cout << "Inventario" << endl;
	createButton(app_, app_->getTextureManager()->getTexture(Resources::BackButton), Vector2D(app_->getWindowWidth() / 2, app_->getWindowHeight() / 2),
		Vector2D(100, 100), goToGame);
}

void InventoryState::goToGame(Application* app)
{
	app->getGameStateMachine()->popState();
}
