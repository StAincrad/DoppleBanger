#include "Resources.h"

vector<Resources::ImageInfo> Resources::imageRoutes{
	//Interfaces
	{MenuFondo, "../Sources/assets/interfaces/menuFondo.png"},
	
	
	{RightArrow,"../Sources/assets/botones/flecha_derecha.png"},
	{LeftArrow, "../Sources/assets/botones/flecha_izquierda.png"},
	{DeleteButton, "../Sources/assets/botones/Borrar_Boton.png"},
	{ Timon, "../Sources/assets/player/timon.png" },
	{ GoControls, "../Sources/assets/botones/botonControles.png"},
	{ Controls, "../Sources/assets/interfaces/controles.png"},
	{ InventaryMenu, "../Sources/assets/interfaces/InventarioFondo.png" },
	{StashMenu, "../Sources/assets/interfaces/AlijoFondo.png"},
	{ Scene1, "../Sources/assets/interfaces/Scene1.png"},
	{ Scene2, "../Sources/assets/interfaces/Scene2.png"},
	{ Scene3, "../Sources/assets/interfaces/Scene3.png"},
	{ TextBox, "../Sources/assets/interfaces/textBoxBg.png" },
	//Sprites
	{ Roca, "../Sources/assets/colisionables/roca.png" },
	//Para añadir nueva ruta: {TextureId, "../Sources/assets/name.png-jpg..."}
	{Dragon,"../Sources/assets/enemigos/dragon.png"},
	//Para añadir nueva ruta: {TextureId, "./Sources/images/name.png/jpg..."}

	//Botones
	{ForwardArrow, "../Sources/assets/botones/FowardArrowButton.png"},
	{BackwardsArrow, "../Sources/assets/botones/BackwardsArrowButton.png"},
	{TrashButton, "../Sources/assets/botones/TrashButton.png"},
	{ChangeButton, "../Sources/assets/botones/ChangeButton.png"},
	{MoneyButton, "../Sources/assets/botones/MoneyButton.png"},
	{CloseButton, "../Sources/assets/botones/Boton_Salir.png"},
	{ BackButton, "../Sources/assets/botones/volver.png"},
	{EquippedButton, "../Sources/assets/botones/Equipar_Boton.png"},
	{BotonMenu, "../Sources/assets/botones/botonMenu.png"},

};

vector<Resources::SoundInfo> Resources::soundRoutes{
	//Para añadir nueva ruta: {AudioId, "./Sources/audio/name.mp3/wav..."}
};

vector <Resources::MusicInfo> Resources::musicRoutes{
	{MainTheme ,"../Sources/assets/music/mainTheme.mp3" }
	//Para añadir nueva ruta: {AudioId, "./Sources/audio/name.mp3/wav..."}
};

vector <Resources::FontInfo> Resources::fontsRoutes{
	{ RETRO, "../Sources/fonts/RetroGaming.ttf", 20 },
	{ RETROSM, "../Sources/fonts/RetroGaming.ttf", 18 }
	//Para añadir nueva ruta: {FontId, "./Sources/fonts/name.ttf", size}
};

vector <Resources::TextMsgInfo> Resources::messages_{

	//Para añadir nuevo texto: {TextureId , string "mensaje", { COLOR(0xaaffffff), FontId  }
	{ PlayText,"Nueva Partida",SDL_Color{(0,0,0,1)},RETRO },
	{ ExitText,"Salir",SDL_Color{(0,0,0,1)},RETRO },
	{ ControlsText,"Controles",SDL_Color{(0,0,0,1)},RETRO },
	{ LoadText,"Cargar Partida",SDL_Color{(0,0,0,1)},RETRO },
	{ CreditsText,"Créditos",SDL_Color{(0,0,0,1)},RETRO },
	{InventoryMoneyText, "Dinero inventario:", SDL_Color({255,255,255,1}), RETRO},
	{StashMoneyText, "Dinero alijo:",  SDL_Color({255,255,255,1}), RETRO}
};
