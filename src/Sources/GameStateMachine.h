#pragma once
#include<stack>
class GameState;
using namespace std;
class GameStateMachine
{
private:
	stack<GameState*> gameStateStack_;
public:
	GameStateMachine() {};
	virtual ~GameStateMachine() {};
	GameState* getState() { return gameStateStack_.top(); };
	void popState() { if (!gameStateStack_.empty())gameStateStack_.pop(); };
	void pushState(GameState* gameState) { gameStateStack_.push(gameState); };
	void changeState(GameState* gameState) { if (!gameStateStack_.empty())gameStateStack_.pop(); gameStateStack_.push(gameState);};
};

