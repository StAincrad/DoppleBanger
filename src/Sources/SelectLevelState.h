#pragma once
#include "GameState.h"
class SelectLevelState :
	public GameState
{
public:
	SelectLevelState(Application* app) :GameState(app) {};
	virtual ~SelectLevelState() {};
	void goToIsland0();
	void goToIsland1();
	void goToIsland2();
};

