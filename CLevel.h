#pragma once
#include <SDL2/SDL.h>
#include "CEngine.h"

class CLevel
{
protected:
	CEngine *mEngine;
	SDL_Renderer *mRenderer;
	TTF_Font *mFont;
	bool mGameOver;

public:
	CLevel();
	virtual ~CLevel();

	virtual bool Update()=0;
	virtual void KeyUp(SDL_Keycode k) = 0;
	virtual void KeyDown(SDL_Keycode k) = 0;
	virtual void StartGame()=0;

	virtual bool InitGame(CEngine *engine, SDL_Renderer *renderer, TTF_Font *font);
	bool IsGameOver() { return(mGameOver); }
};

