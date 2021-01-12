#include "CEngine.h"


CEngine::CEngine()
{
	mBackground = NULL;
	mRenderer = NULL;
}

CEngine::CEngine(SDL_Renderer *renderer) : CEngine()
{
	mRenderer = renderer;
	mNumSprites = 0;
}

void CEngine::Tick()
{
	int i;
	for (i = 0; i < mNumSprites; i++)
	{
		mSprites[i]->Tick();
		mSprites[i]->Move();
	}
}

void CEngine::Render()
{
	int i;
	if (mBackground != NULL && mRenderer!=NULL)
	{
		mBackground->Draw(mRenderer);
		for (i = 0; i < mNumSprites; i++)
		{
			if (mSprites[i] != NULL)
			{
				mSprites[i]->Draw(mRenderer);
			}
		}
	}
}

void CEngine::AddSprite(CAnimate *s)
{
	mSprites[mNumSprites] = s;
	mNumSprites++;
}

void CEngine::SetBG(CTexture *bg)
{
	mBackground = bg;
}

void CEngine::Clear()
{
	int i;
	for (i = 0; i < 100; i++)
	{
		mSprites[i]=NULL;
	}
	mNumSprites = 0;
	mBackground = NULL;
}

void CEngine::ShowHitBox(bool b)
{
	int i;

	for (i = 0; i < mNumSprites; i++)
	{
		if (mSprites[i] != NULL)
		{
			mSprites[i]->ShowHitBox(b);
		}
	}
}