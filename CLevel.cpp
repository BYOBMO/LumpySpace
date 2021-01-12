#include "CLevel.h"



CLevel::CLevel()
{
	mEngine = NULL;
	mRenderer = NULL;
	mFont = NULL;
	bool mGameOver = true;
}


CLevel::~CLevel()
{
}

bool CLevel::InitGame(CEngine *engine, SDL_Renderer *renderer, TTF_Font *font)
{
	return(true);
}
