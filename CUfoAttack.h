#pragma once
#include <SDL2/SDL.h>
#include "CLevel.h"
#include "CPlayer.h"
#include "CUfo.h"
#include "CText.h"

class CUfoAttack : public CLevel
{
private:
	//The window renderer

	int mReloadCounter;
	int mFloor;
	bool mDone;

	Uint32 mLastUfo;
	int mUfoPlacementTimer;

	CTexture bg;			// Main background texture.
	CTexture title;			// Game over title image.
	CTexture missileTex;
	CTexture bombTex;
	CTexture bulletTex;
	CTexture ufoTex;
	CTexture playerTex;
	CTexture jakeBugTex;
	CTexture eggTex;
	CTexture alienTex;
	CTexture explosionTex;
	CTexture splatTex;
	CTexture progressTex;
	CTexture progressBlankTex;

	CPlayer *mPlayer = NULL;
	CAnimate *mJakeBug = NULL;

	CUfo *ufos[10];
	CEgg *eggs[10];
	CBomb *bombs[10];
	CAlien *aliens[10];
	CBullet *bullets[10];
	CMissile *missiles[10];
	CAnimate *mExplosions[20];
	CAnimate *mSplats[5];

	CAnimate *mMissilesLeft;		// Progress bar for number of missiles left.
	CAnimate *mMissilesLeftBlank;	// Empty progress bar (gray)
	CAnimate *mMissileIcon;			// Icon displayed next to the missile progress bar.

	CAnimate *mTitle;				// Game over title animation.
	CText *mScoreText;
	CText *mScoreTextValue;
	CText *mLevelText;
	CText *mLevelTextValue;
	CText *mShipsText;
	CText *mShipsTextValue;
	CText *mLevelHeader;			// Displayed when a new level starts/

	SDL_Color textBlack = { 0, 0, 0 };
	SDL_Color textYellow = { 255, 255, 0 };
	SDL_Color textWhite = { 255, 255, 255 };

	Mix_Chunk *gGunSound = NULL;
	Mix_Chunk *gLaserSound = NULL;
	Mix_Chunk *gExplosionSound = NULL;
	Mix_Chunk *gUfoExplosion = NULL;
	Mix_Chunk *gEggHatch = NULL;
	Mix_Chunk *gAlienDeath = NULL;
	Mix_Chunk *gLayEgg = NULL;
	Mix_Chunk *gEatEgg = NULL;
	Mix_Chunk *gUfoRez = NULL;
	Mix_Chunk *gSaveLsp = NULL;
	Mix_Chunk *gJakeBeat = NULL;

	int mLevel;
	int mPlayersLeft;
	int mNumUfos;
	int mActiveUfos;
	int mUfoCtr;

	int mScore;
	int mUfosLeft;


	void PlaceExplosion(int x, int y);
	void PlaceSplat(int x, int y);
	void Rules();
	void StartPlayer();
	void LevelUp();
	void CheckUfos();
	void PlaceUfo(CUfo *ufo);
	void ResetAttack();
	void WeaponsFree();
	void WeaponsHold();
	void PlayerUp();
	void PlayerDown();
	void GameOver();

public:
	CUfoAttack();
	~CUfoAttack();

	bool InitGame(CEngine *engine, SDL_Renderer *renderer, TTF_Font *font);
	bool Update();
	void KeyUp(SDL_Keycode k);
	void KeyDown(SDL_Keycode k);
	void StartGame();




};

