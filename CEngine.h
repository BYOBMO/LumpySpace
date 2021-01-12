#pragma once
#include "CAnimate.h"
#include "CTexture.h"
#include <SDL2/SDL.h>

class CEngine
{
	CTexture *mBackground;

	SDL_Renderer *mRenderer;
	CAnimate *mSprites[100];
	int mNumSprites;

public:
	CEngine();
	CEngine(SDL_Renderer *renderer);
	void SetBG(CTexture *bg);
	void Tick();
	void Render();
	void AddSprite(CAnimate *s);
	void Clear();
	void ShowHitBox(bool b);
};

