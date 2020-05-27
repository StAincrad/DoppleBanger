#include "ShipState.h"
#include "ShipObject.h"
#include "Player.h"
#include "GameManager.h"
#include "SaveState.h"
#include "StashState.h"
#include "SelectLevelState.h"
#include "CaribbeanIslandState.h"
#include "SpookyIslandState.h"
#include "VolcanicIslandState.h"
#include "TestState.h"


#pragma region CallBacks
//Callback para cambiar de GameState e ir a la isla actual
void ShipState::goIsland(Application* app)
{
	//Viajamos a la isla correspondiente
	GameManager* gm = GameManager::instance();
	Island currIsland = gm->getCurrIsland();
	if (currIsland == Island::Caribbean) app->getGameStateMachine()->changeState(new SpookyIslandState(app));
	else if (currIsland == Island::Spooky) app->getGameStateMachine()->changeState(new SpookyIslandState(app));
	else if (currIsland == Island::Volcanic) app->getGameStateMachine()->changeState(new VolcanicIslandState(app));
}
//Callback del alijo para ir al men� de alijo
void ShipState::goStashState(Application* app)
{
	app->getGameStateMachine()->pushState(new StashState(app));
}
//Callback del mapa
void ShipState::goMap(Application* app)
{
	app->getGameStateMachine()->pushState(new SelectLevelState(app));
}
//Callback para ir al menu de guardado
void ShipState::goSaveState(Application* app)
{
	app->getGameStateMachine()->pushState(new SaveState(app));
	app->resetMusicChannels();
	app->getAudioManager()->playChannel(Resources::MainTheme, -1, Resources::MainMusicChannel);
}
#pragma endregion

void ShipState::initState()
{
	//Borramos la lista de objetos del barco del CollisionCtrl
	collisionCtrl_->clearList();
	gm_->setOnShip(true);

	collisionMap_ = new TiledMap(app_, this, TILED_MAP, TILE_HEIGHT_TILESET, TILE_WIDTH_TILESET,TILE_DRAWING_SIZE, app_->getTextureManager()->getTexture(Resources::CollisionTile),
		TILESET_FILS, TILESET_COLS, Vector2D(0, 0));

	background_ = new Draw(app_, app_->getTextureManager()->getTexture(Resources::Ship));
	addRenderUpdateLists(background_);

	SDL_Rect destRect; //Rect�ngulo para los objetos

	//Creaci�n del alijo
	destRect.w = (int)round(W_STASH); destRect.h = (int)round(H_STASH);
	destRect.x = (int)round(W_WIN / 2); destRect.y = (int)round(H_WIN * 2 / 5);
	stash_ = new ShipObject(app_, Vector2D(destRect.x, destRect.y), Vector2D(destRect.w, destRect.h),
		app_->getTextureManager()->getTexture(Resources::Stash), goStashState);
	addRenderUpdateLists(stash_);
	addObject(stash_);

	//Creaci�n de la trampilla
	destRect.w =(int)round( W_DOOR); destRect.h =(int)round(H_DOOR);
	destRect.x =(int)round( (W_WIN / 2) - W_DOOR * 1.5); destRect.y =(int)round( H_WIN * 2 / 5 + H_WHEEL / 2);
	door_ = new ShipObject(app_, Vector2D(destRect.x, destRect.y), Vector2D(destRect.w, destRect.h),
		app_->getTextureManager()->getTexture(Resources::ShipDoor), goSaveState);
	addRenderUpdateLists(door_);
	addObject(door_);

	//Creaci�n del timon
	destRect.w = (int)round(W_WHEEL); destRect.h =(int)round( H_WHEEL);
	destRect.x = (int)round((W_WIN / 2) - W_WHEEL / 2); destRect.y =(int)round( H_WIN / 7);
	wheel_ = new ShipObject(app_, Vector2D(destRect.x, destRect.y), Vector2D(destRect.w, destRect.h),
		app_->getTextureManager()->getTexture(Resources::Wheel), goMap);
	addRenderUpdateLists(wheel_);
	addObject(wheel_);
	
	//Creacion de la salida
	destRect.w = (int)round(W_EXIT); destRect.h = (int)round(H_EXIT);
	destRect.x = (int)round(W_WIN - W_EXIT); destRect.y = (int)round(H_WIN * 2 / 3);
	exit_ = new ShipObject(app_, Vector2D(destRect.x, destRect.y), Vector2D(destRect.w, destRect.h),
		app_->getTextureManager()->getTexture(Resources::ExitShip), goIsland);
	addRenderUpdateLists(exit_);
	addObject(exit_);

	collisionCtrl_->addShipObjects(stash_, door_, wheel_, exit_);

	//Posición de Venancio
	POST_TUTORIAL_POS = Vector2D((double)(W_WIN * 2 / 7), (double)(H_WIN / 10));
	TUTORIAL_POS = Vector2D((double)(destRect.x - W_WIN / 8), (double)(destRect.y - H_WIN / 9));

	createNPCs();

	////Siempre se a�ade el �ltimo para que se renderice por encima de los dem�s objetos
	playerEntry_= Vector2D((W_WIN - W_PLAYER * 4), ((H_WIN * 3 / 4) - H_PLAYER));
	player_ = new Player(app_, playerEntry_, Vector2D(W_PLAYER, H_PLAYER));
	player_->initSkills();
	player_->setScale(Vector2D(W_PLAYER, H_PLAYER));
	player_->setColliderPos(Vector2D((player_->getScale().getX() / 3), 2 * (player_->getScale().getY() / 4)));
	player_->setColliderScale(Vector2D((player_->getScale().getX() / 3), (player_->getScale().getY() / 4)));
	gm_->setCurrentPlayerLife(player_->getHealth());
	gm_->setCurrentPlayerMana(player_->getMana());


	Camera::instance()->updateCamera((int)round(W_WIN) / 2, (int)round(H_WIN) / 2);
	addRenderUpdateLists(player_);

	app_->resetMusicChannels();
	app_->resetSoundsChannels();
	app_->getAudioManager()->playChannel(Resources::ShipTheme, -1, 0);
}

void ShipState::update()
{

#ifdef _DEBUG
	if (eventHandler_->isKeyDown(SDL_SCANCODE_BACKSPACE))
		app_->getGameStateMachine()->changeState(new TestState(app_));
	else {
#endif
		PlayState::update();
		collisionCtrl_->shipCollisions();
#ifdef _DEBUG
	}
#endif // _DEBUG

}

void ShipState::loadState(){
	//Para cargar los npc
	if (gm_->isThatMissionPass(missions::gallegaEnProblemas)) {
		NPC* chef;
		chef = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Cooker), Vector2D(W_WIN / 10, H_WIN / 35), Vector2D(W_CHEF, H_CHEF), 2);
		addRenderUpdateLists(chef);
	}

	if (gm_->isThatMissionPass(missions::papelesSiniestros)) {
		NPC* morty;
		morty = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Morty), Vector2D(W_WIN * 8 / 9, H_WIN / 2), Vector2D(W_MORTY, H_MORTY), 3);
		addRenderUpdateLists(morty);
	}

	if (gm_->isThatMissionPass(missions::masValePajaroEnMano)) {
		NPC* parrot;
		parrot = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Parrot), Vector2D(W_WIN / 4, (double)H_WIN * 7 / 16), Vector2D(W_PARROT, H_PARROT), 4);
		addRenderUpdateLists(parrot);
	}

	if (gm_->isThatMissionPass(missions::laboon)) {
		NPC* skeleton;
		skeleton = new NPC(app_, app_->getTextureManager()->getTexture(Resources::SkeletonMusician), Vector2D(W_WIN * 3 / 5, H_WIN / 7), Vector2D(W_SKELETON, H_SKELETON), 5);
		addRenderUpdateLists(skeleton);
	}

	if (gm_->isThatMissionPass(missions::arlongPark)) {
		NPC* cartographer;
		cartographer = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Cartographer), Vector2D((double)W_WIN * 11 / 14, H_WIN / 30), Vector2D(W_CARTOGRAPHER, H_CARTOGRAPHER), 6);
		addRenderUpdateLists(cartographer);
	}
}

void ShipState::createNPCs() {
	//El viejo y el mercader están desde el principio
	venancio_ = new NPC(app_, app_->getTextureManager()->getTexture(Resources::VenancioDrink), POST_TUTORIAL_POS, Vector2D(W_ELDERMAN, H_ELDERMAN), 0);
	addRenderUpdateLists(venancio_);

	NPC* merchant;
	merchant = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Merchant), Vector2D(W_WIN / 30, H_WIN * 3 / 5), Vector2D(W_MERCHANT, H_MERCHANT), 1);
	addRenderUpdateLists(merchant);

	//Comprobamos si los NPCs restantes están desbloqueados
	if (gm_->isThatMissionPass(missions::gallegaEnProblemas)) {
		NPC* chef;
		chef = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Cooker), Vector2D(W_WIN / 10, H_WIN / 35), Vector2D(W_CHEF, H_CHEF), 2);
		addRenderUpdateLists(chef);
	}

	if (gm_->isThatMissionPass(missions::papelesSiniestros)) {
		NPC* morty;
		morty = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Morty), Vector2D(W_WIN *10 / 12, H_WIN / 4), Vector2D(W_MORTY, H_MORTY), 3);
		addRenderUpdateLists(morty);
	}

	if (gm_->isThatMissionPass(missions::masValePajaroEnMano)) {
		NPC* parrot;
		parrot = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Parrot), Vector2D(W_WIN / 4, (double)H_WIN * 7 / 16), Vector2D(W_PARROT, H_PARROT), 4);
		addRenderUpdateLists(parrot);
	}

	if (gm_->isThatMissionPass(missions::laboon)) {
		NPC* skeleton;
		skeleton = new NPC(app_, app_->getTextureManager()->getTexture(Resources::SkeletonMusician), Vector2D(W_WIN * 3 / 5, H_WIN / 7), Vector2D(W_SKELETON, H_SKELETON), 5);
		addRenderUpdateLists(skeleton);
	}

	if (gm_->isThatMissionPass(missions::arlongPark)) {
		NPC* cartographer;
		cartographer = new NPC(app_, app_->getTextureManager()->getTexture(Resources::Cartographer), Vector2D((double)W_WIN * 11 / 14, H_WIN / 30), Vector2D(W_CARTOGRAPHER, H_CARTOGRAPHER), 6);
		addRenderUpdateLists(cartographer);
	}
}