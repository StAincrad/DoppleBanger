#include "StashState.h"

void backToPrevious(Application* app) {
	app->getGameStateMachine()->popState();
}

void StashState::initState()
{
	createButton(app_, app_->getTextureManager()->getTexture(Resources::BackButton), Vector2D(app_->getWindowWidth() / 2, app_->getWindowHeight() / 2),
		Vector2D(300, 300), backToPrevious);
}
