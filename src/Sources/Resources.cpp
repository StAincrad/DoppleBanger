#include "Resources.h"

vector<Resources::ImageInfo> Resources::imageRoutes{
	//Interfaces
	{MenuFondo, "../Sources/assets/interfaces/menuFondo.png"},
	{BotonMenu, "../Sources/assets/botones/botonMenu.png"},
	{ Timon, "../Sources/assets/player/timon.png" },
	{ GoControls, "../Sources/assets/botones/botonControles.png"},
	{ BackButton, "../Sources/assets/botones/volver.png"},
	{ Controls, "../Sources/assets/interfaces/controles.png"},
	{ InventaryMenu, "../Sources/assets/interfaces/InventarioFondo.png" },
	{ Scene1, "../Sources/assets/interfaces/Scene1.png"},
	{ Scene2, "../Sources/assets/interfaces/Scene2.png"},
	{ Scene3, "../Sources/assets/interfaces/Scene3.png"},
	{ TextBox, "../Sources/assets/interfaces/textBoxBg.png" },
	//Sprites
	{ Roca, "../Sources/assets/colisionables/roca.png" },
	//Para añadir nueva ruta: {TextureId, "../Sources/assets/name.png-jpg..."}
	{Dragon,"../Sources/assets/enemigos/dragon.png"},
	{MonkeyCoco,"../Sources/assets/enemigos/MonkeyCoco.png"},
	{Coco ,"../Sources/assets/Colisionables/Coco.png" }

	//Para añadir nueva ruta: {TextureId, "./Sources/images/name.png/jpg..."}
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
};
