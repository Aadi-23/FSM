#pragma once
#include "Level.h"
#include "stack"
#include"GUI.h"


enum GAMESTATES
{
	MENUSCREEN,
	GAMESCREEN,
	PAUSESCREEN,
	ENDSCREEN,
};


struct GameManager
{
	std::stack<GAMESTATES>gamestate;

	Level level;

	bool isGameRunning = true;

	GAMESTATES currentgamestate = {};

	void GameLoop();
	void DoMenuFrame();
	
};