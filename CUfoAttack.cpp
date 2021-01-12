#include "CUfoAttack.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <string>
#include <iostream>  
#include <iomanip>  

#include "CTexture.h"
#include "CTimer.h"
#include "CAnimation.h"
#include "CParaMove.h"
#include "CMissile.h"
#include "CBullet.h"
#include "CXMove.h"
#include "CBoxBounce.h"
#include "CAlien.h"
#include "CEngine.h"
#include "CPlayer.h"
#include "CEgg.h"
#include "CBomb.h"
#include "CUfo.h"
#include "CRules.h"
#include "CText.h"

using namespace std;


CUfoAttack::CUfoAttack() : CLevel()
{
	// After a UFO dies, wait 2 seconds before rezzing it.
	mUfoPlacementTimer = 2000;
}


CUfoAttack::~CUfoAttack()
{
}


void CUfoAttack::GameOver()
{
	int i;

	mGameOver = true;
	ResetAttack();
	for (i = 0; i<7; i++)
	{
		CUfo *ufo = ufos[i];
		if (ufo != NULL)
		{
			if (i>0)
			{
				ufo->mState = 0;
				ufo->MoveOffScreen();
				ufo->Reset();
			}
		}
	}
	mPlayer->Reset();
	WeaponsHold();
	mTitle->mView = true;
	mJakeBug->mView = true;
	mReloadCounter = -1;
	mActiveUfos = 0;
	Mix_PlayChannel(1, gJakeBeat, 3);
}

void CUfoAttack::PlayerDown()
{
	mPlayer->mActive = false;
	mPlayer->Start(3, false);
	mPlayer->SetState(1);
	mReloadCounter = 300; // Wait before player re-rezzes.

	WeaponsHold();
	mPlayersLeft--;

	if (mPlayersLeft == 0)
		GameOver();
}

void CUfoAttack::PlayerUp()
{
	mPlayer->Reset();

	mPlayer->SetState(0);
	mPlayer->SetVelocity(0);
	mPlayer->SetPos(bg.GetWidth() / 2 - mPlayer->GetWidth() / 2, mFloor - mPlayer->GetHeight());
	mPlayer->SetMoverState(true);
	mPlayer->mActive = true;
	mReloadCounter = -1;
	if (mLevel == 1)
	{
		mPlayer->mNumMissilesLeft = -1;
	}
	else
	{
		mPlayer->mMaxMissiles = 10 + mLevel * 3;
		if (mPlayer->mMaxMissiles > 30)
		{
			mPlayer->mMaxMissiles = 30;
		}
		mPlayer->mNumMissilesLeft = mPlayer->mMaxMissiles;

	}

	WeaponsFree();
}

void CUfoAttack::WeaponsHold()
{
	int i;

	for (i = 0; i<7; i++)
	{
		CUfo *ufo = ufos[i];
		ufo->mWeaponsFree = false;
	}
}

void CUfoAttack::WeaponsFree()
{
	int i;
	for (i = 0; i<7; i++)
	{
		CUfo *ufo = ufos[i];
		if (ufo != NULL)
		{
			ufo->mWeaponsFree = true;
		}
	}
}

void CUfoAttack::ResetAttack()
{
	int i;
	for (i = 0; i<7; i++)
	{
		CBomb *bomb = bombs[i];
		CEgg *egg = eggs[i];
		CAlien *alien = aliens[i];
		bomb->Reset();
		egg->Reset();
		alien->Reset();
	}
}

void CUfoAttack::StartGame()
{
	mGameOver = false;
	mLevel = 0;
	mScore = 0;
	mNumUfos = 1;
	mPlayersLeft = 4;
	LevelUp();
	mTitle->mView = false; // Turn off game over graphics.
	mJakeBug->mView = false;
	
	mReloadCounter = 300;

	if (gSaveLsp != NULL)
	{
		Mix_HaltChannel(1);
		Mix_PlayChannel(-1, gSaveLsp, 0);
	}
}

void CUfoAttack::PlaceUfo(CUfo *ufo)
{
	int x, y;


	x = 30 + (int)((float)600 * CRules::GetRandom());
	y = 20 + (int)((float)250 * CRules::GetRandom());
	ufo->SetPos(x, y);
	ufo->Start(1, false);
	ufo->SetState(1);
	ufo->SetMoverState(true);
	int dir = 1;
	if ((rand() % 2) == 0) // Pick a random direction.
		dir = -1;

	ufo->SetVelocity((1 + rand() % 3) * dir);
	ufo->mActive = true;
	ufo->SetBombRate(2000);

}

//
// Place an explosion on the screen.
//
void CUfoAttack::PlaceExplosion(int x, int y)
{
	int i;
	for (i = 0; i<20; i++)
	{
		CAnimate *ex = mExplosions[i];

		// Find an avaialbe animation then place it.
		if (ex->mActive == false)
		{
			ex->SetPos(x, y);
			ex->Start(rand()%5, false); // Randomly pick a color.
			return;
		}
	}
}

//
// Place an explosion on the screen.
//
void CUfoAttack::PlaceSplat(int x, int y)
{
	int i;
	for (i = 0; i<5; i++)
	{
		CAnimate *ex = mSplats[i];

		// Find an avaialbe animation then place it.
		if (ex->mActive == false)
		{
			ex->SetPos(x, y);
			ex->Start(0, false); // Randomly pick a color.
			return;
		}
	}
}

//
// See if any UFO's need placing. If one deos get palced, then 
// wait 500 milliseconds before placing another one to stagger the rezzing effects.
//
void CUfoAttack::CheckUfos()
{
	int i;

	if (mLevelHeader->mView)
	{
		return;
	}

	if (mUfosLeft <= 0)
	{
		// Level is over, move UFOs off screen.
		mPlayer->MoveOffScreen();
		mPlayer->mActive = false;

		LevelUp();
	}
	else
	{
		// See if the placement timer has expired.
		Uint32 ticks = SDL_GetTicks();

		mUfoPlacementTimer = mUfoPlacementTimer - (ticks - mLastUfo);
		mLastUfo = ticks;

		if (mUfoPlacementTimer <= 0)
		{
			mUfoPlacementTimer = 0;
			// Look for a UFO that needs placement/
			for (i = 0; i < mActiveUfos; i++)
			{
				CUfo *ufo = ufos[i];
				if (ufo->mActive == false)
				{
					PlaceUfo(ufo);
					mUfoPlacementTimer = 500; // UFO is placed. now wait 500 miliseconds before doing the next one.
					break;
				}
			}
		}
	}
}


//
// Adjust difficulties for the next level.
//
void CUfoAttack::LevelUp()
{
	int i;
	mLevel++;
	mNumUfos++;

	mLastUfo = SDL_GetTicks(); // zero the placement timer for UFOs.

	mUfosLeft = 9 + mLevel;
	mActiveUfos = 0;

	if (mNumUfos > 7)
		mNumUfos = 7;

	for (i = 0; i<3; i++)
	{
		// Reset all the player missiles.
		CMissile *missile = missiles[i];
		missile->mActive = false;
		missile->Reset();
		missile->MoveOffScreen();
	}

	mUfoPlacementTimer = 2000; //Start the UFO placement timer.

	for (i = 0; i < 7; i++)
	{
		// Reset all UFOs
		CUfo *ufo = ufos[i];
		ufo->mState = 0;
		ufo->MoveOffScreen();
		ufo->Reset();
	}

	if (mLevel == 4)
	{
		CEgg::mHatchTimeBase = 1500;
		CEgg::mHatchTimeRange = 500;
	}	
	else if (mLevel == 5)
	{
		CEgg::mHatchTimeBase = 1000;
		CEgg::mHatchTimeRange = 1;
	}
	else if (mLevel >= 4)
	{
		CEgg::mHatchTimeBase = 0;
		CEgg::mHatchTimeRange = 1;
	}


	for (i = 0; i<7; i++)
	{
		// Reinitialize all UFOs.
		CUfo *ufo = ufos[i];

		if (i>mNumUfos)
		{
			ufo->mState = 0;
			ufo->MoveOffScreen();
			ufo->Reset();
		}
		else
		{
			mActiveUfos++;
			
			//PlaceUfo(ufo);
		}

		if (mLevel == 1)
		{
			// Level 1 has no eggs.
			ufo->DropEgg(false);
			ufo->EggHatch(false);
		}
		else if (mLevel == 2)
		{
			// Level 2 has eggs but they don't hatch
			ufo->DropEgg(true);
			ufo->EggHatch(false);
		}
		else
		{
			// All levels now have hatching eggs.
			ufo->DropEgg(true);
			ufo->EggHatch(true);
		}
	}

	StartPlayer();
	WeaponsHold(); // Hold weapons until countdown timer starts the level.
}

void CUfoAttack::StartPlayer()
{
	// Move the player off screen. Start the reload counter. The player will appear when the counter hits 0.
	mPlayer->MoveOffScreen();
	mReloadCounter = 300;
	mLevelHeader->mView = true;
}


//
// handles all of the game logic and collision detection.
//
void CUfoAttack::Rules()
{
	int i, j;

	CheckUfos();

	// Check of missiles hitting the UFOs.
	for (i = 0; i<3; i++)
	{
		CRectangle r1, r2;
		CMissile *missile = missiles[i];
		//				int st = missile.GetState();

		if (missile->mActive)
		{

			r2 = missile->GetBounds();
			if (mPlayer->mActive && mPlayer->Collide(r2) && missile->GetState() == 1)
			{
				PlayerDown();
				mScore -= 5000;
			}
			else
			{
				for (j = 0; j<7; j++)
				{
					CUfo *ufo = ufos[j];


					r2 = missile->GetBounds();
					if (ufo->Collide(r2))
					{
						if (ufo->GetState() != 1)
						{
							mScore += 1000;
							missile->Reset();
							PlaceExplosion(ufo->GetPosX(), ufo->GetPosY());
							ufo->MoveOffScreen();
							ufo->Reset();
							mUfosLeft--;
							if (mUfoPlacementTimer == 0)
								mUfoPlacementTimer = 2000; // Rez a new UFO after 2 seconds if one isn't already being rezzed.

							break;
						}
					}
				}
			}
		}
	}

	// Check for UFO bombs hitting the player.
	for (i = 0; i<7; i++)
	{
		CBomb *bomb = bombs[i];

		if (bomb->mActive)
		{
			CRectangle r = bomb->GetBounds();
			if (mPlayer->mActive && mPlayer->Collide(r))
			{
				bomb->Reset();
				PlayerDown();
				break;
			}
		}
		else
		{
		}
	}

	// Check for bullets hitting alien scum.
	for (i = 0; i<3; i++)
	{
		CBullet* bullet = bullets[i];

		if (bullet->mActive)
		{
			for (j = 0; j<7; j++)
			{
				CAlien *alien;
				CEgg *egg = eggs[j];
				alien = egg->GetAlien();

				CRectangle r1, r2;

				r2 = alien->GetBounds();
				r1 = egg->GetBounds();
				if (egg->mActive==false && bullet->Collide(r2))
				{
					if (alien->GetState()<2)
					{
						mScore += 100;
						bullet->Reset();
						PlaceSplat(alien->GetPosX(), alien->GetPosY());
						//alien->Start(3, false);
						//alien->SetState(2);
						alien->Reset();
						alien->MoveOffScreen();
						break;
					}
				}
				else if (bullet->Collide(r1))
				{
					bullet->Reset();
					break;
				}
			}
		}
	}

	// Check for player eating eggs.
	for (i = 0; i<7; i++)
	{
		CEgg *egg = eggs[i];
		CAlien *alien = NULL;

		if (egg->mActive)
		{
			CRectangle r = egg->GetBounds();
			if (mPlayer->Collide(r))
			{
				mScore += 500;
				Mix_PlayChannel(-1, gEatEgg, 0);
				egg->Reset();
				egg->ResetAlien();
				mPlayer->mNumMissilesLeft += 3;
				if (mPlayer->mNumMissilesLeft > mPlayer->mMaxMissiles)
				{
					mPlayer->mNumMissilesLeft = mPlayer->mMaxMissiles;
				}
				break;
			}
		}
		else
		{
			// Ignore the alien if an egg is visible.
			alien = egg->GetAlien();
		}

		// Check for alien scum eating player.
		if (alien != NULL)
		{
			if (alien->mActive && alien->GetState() < 2)
			{
				CRectangle r = alien->GetBounds();
				if (mPlayer->mActive && mPlayer->Collide(r))
				{
					alien->Reset();
					PlayerDown();
					break;
				}
			}
		}
	}

}

//
// Load all the game assests.
//
bool CUfoAttack::InitGame(CEngine *engine, SDL_Renderer *renderer, TTF_Font *font)
{
	int i, x, y;

	mEngine = engine;
	mRenderer = renderer;
	mFont = font;

	mReloadCounter = -1;
	mFloor = 550;

	for (i = 0; i<10; i++)
	{
		ufos[i] = NULL;
		missiles[i] = NULL;
		bullets[i] = NULL;
		eggs[i] = NULL;
		aliens[i] = NULL;
		bombs[i] = NULL;
	}

	mDone = false;

	gGunSound = Mix_LoadWAV("sounds/fart.wav");
	if (gGunSound == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gLaserSound = Mix_LoadWAV("sounds/poof.wav");
	if (gLaserSound == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	gExplosionSound = Mix_LoadWAV("sounds/JakeAh.wav");
	if (gExplosionSound == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gSaveLsp = Mix_LoadWAV("sounds/savelsp.wav");
	if (gSaveLsp == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}


	gJakeBeat = Mix_LoadWAV("sounds/beat2_01.wav");
	if (gJakeBeat == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}


	gUfoExplosion = Mix_LoadWAV("sounds/expl3.wav");
	if (gUfoExplosion == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gEggHatch = Mix_LoadWAV("sounds/crack.wav");
	if (gEggHatch == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gEatEgg = Mix_LoadWAV("sounds/eategg.wav");
	if (gEatEgg == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gUfoRez = Mix_LoadWAV("sounds/uforez.wav");
	if (gUfoRez == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gLayEgg = Mix_LoadWAV("sounds/layegg.wav");
	if (gLayEgg == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gAlienDeath = Mix_LoadWAV("sounds/alien.wav");
	if (gAlienDeath == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	bg.LoadFromFile(mRenderer, "graphics/LS_Background_path.gif");
	title.LoadFromFile(mRenderer, "graphics/LS_Title.gif");

	explosionTex.LoadFromFile(mRenderer, "graphics/multiexplosion.png");
	splatTex.LoadFromFile(mRenderer, "graphics/splat2.png");


	missileTex.LoadFromFile(mRenderer, "graphics/LS_Missle.gif");
	missileTex.SetBounds(CRectangle{ 2, 2, 8, 19 });

	bombTex.LoadFromFile(mRenderer, "graphics/LS_bomb.gif");
	bombTex.SetBounds(CRectangle{ 0, 0, 8, 16 });

	bulletTex.LoadFromFile(mRenderer, "graphics/LS_Bullet.gif");
	bulletTex.SetBounds(CRectangle{ 0, 0, 10, 10 });

	eggTex.LoadFromFile(mRenderer, "graphics/LS_egg.gif");
	eggTex.SetBounds(CRectangle{ 20, 5, 20, 34 });

	ufoTex.LoadFromFile(mRenderer, "graphics/LS_ufo2.png");
	ufoTex.SetBounds(CRectangle{ 10, 10, 60, 20 });

	alienTex.LoadFromFile(mRenderer, "graphics/LS_alien.png");
	alienTex.SetBounds(CRectangle{ 5, 15, 31, 25 });

	playerTex.LoadFromFile(mRenderer, "graphics/LS_player_large3.gif");
	playerTex.SetBounds(CRectangle{ 20, 19, 104, 37 });

	progressTex.LoadFromFile(mRenderer, "graphics/LS_progress.png");
	progressBlankTex.LoadFromFile(mRenderer, "graphics/LS_progressBlank.png");

	jakeBugTex.LoadFromFile(mRenderer, "graphics/jakebug5x3.png");


	mEngine->SetBG(&bg);


	mJakeBug = new CAnimate();
	mJakeBug->SetTexture(&jakeBugTex, 3, 5);
	mJakeBug->mWrapAnimation = true;
	mJakeBug->mLoop = true;
	mJakeBug->SetFrameRate(80);
	mEngine->AddSprite(mJakeBug);
	mJakeBug->SetPos(bg.GetWidth() / 2 - mJakeBug->GetWidth() / 2, 400);
	mJakeBug->mView = true;
	mJakeBug->Start(0, true);
	Mix_PlayChannel(1, gJakeBeat, 10);

	mPlayer = new CPlayer();

	mPlayer->SetTexture(&playerTex, 4, 6);
	mPlayer->SetMaxFrames(3, 3, 3, 6, 0, 0, 0, 0, 0, 0);
	mPlayer->SetMover(new CXMove(0, bg.GetWidth(), 1));
	mPlayer->MoveOffScreen();
	mPlayer->AddAudio(gLaserSound);
	mPlayer->AddAudio(gGunSound);
	mPlayer->AddAudio(gGunSound);
	mPlayer->AddAudio(gExplosionSound);

	mEngine->AddSprite(mPlayer);

	CMissile *missileObj;
	CBullet *bulletObj;

	mPlayersLeft = 4;
	mLevel = 1;

	for (i = 0; i<3; i++)
	{
		missileObj = new CMissile();
		missileObj->SetTexture(&missileTex, 1, 2);
		missileObj->Stop();
		missileObj->SetVelocity(2);
		missileObj->SetPos(100, 340);
		missileObj->MoveOffScreen();
		mEngine->AddSprite(missileObj);
		missileObj->SetMover(new CParaMove(mFloor - mPlayer->GetHeight(), -10, 0, false, 300));
		mPlayer->AddMissile(missileObj);
		missileObj->SetMoverState(false);
		missiles[i] = missileObj;
	}

	for (i = 0; i<3; i++)
	{
		bulletObj = new CBullet();
		bulletObj->SetTexture(&bulletTex, 1, 8);
		bulletObj->Stop();
		bulletObj->SetVelocity(10);
		bulletObj->SetPos(-1, mFloor - 18);
		bulletObj->MoveOffScreen();

		mEngine->AddSprite(bulletObj);
		bulletObj->SetMover(new CXMove(0, bg.GetWidth(), 3));
		mPlayer->AddBullet(bulletObj);
		bulletObj->SetMoverState(false);
		bullets[i] = bulletObj;
	}



	CBomb *bombObj;
	CEgg *eggObj;

	for (i = 0; i<7; i++)
	{
		x = 30 + (int)((float)400 * CRules::GetRandom());
		y = 20 + (int)((float)200 * CRules::GetRandom());
		CUfo *u = new CUfo();
		//u->SetMaxFrames(8, 3, 0, 0, 0, 0, 0, 0, 0, 0);
		u->SetTexture(&ufoTex, 2, 8);
		u->Start(0, true);
		u->SetVelocity(1);
		u->SetPos(x, y);
		mEngine->AddSprite(u);
		u->SetMover(new CBoxBounce(20, 20, 770, 400));
		u->mActive = true;
		u->SetBombRate(2000);
		u->AddAudio(NULL);
		u->AddAudio(gUfoRez);

		eggObj = new CEgg();
		eggObj->SetTexture(&eggTex, 2, 10);
		eggObj->SetMaxFrames(1, 10, 0, 0, 0, 0, 0, 0, 0, 0);
		eggObj->Start(0, true);
		eggObj->SetFloor(mFloor - eggObj->GetHeight());
		eggObj->MoveOffScreen();
		eggObj->SetMover(new CParaMove(eggObj->GetFloor(), 0, 1100, true, 2000));
		eggObj->mActive = false;
		eggObj->mLoop = false;
		eggObj->SetFrameRate(100);
		eggObj->SetVelocity(1);
		eggObj->AddAudio(gLayEgg);
		eggObj->AddAudio(gEggHatch);

		CAlien *alienObj = new CAlien();
		alienObj->AddAudio(NULL);
		alienObj->AddAudio(NULL);
		alienObj->AddAudio(NULL);
		alienObj->AddAudio(gAlienDeath);

		alienObj->SetTexture(&alienTex, 4, 7);
		alienObj->SetMaxFrames(7, 3, 3, 4, 0, 0, 0, 0, 0, 0);
		alienObj->Start(0, false);

		alienObj->SetPos(0, mFloor - alienObj->GetHeight());
		alienObj->MoveOffScreen();
		alienObj->SetMover(new CXMove(-50, bg.GetWidth(), 3));
		alienObj->mActive = false;
		mEngine->AddSprite(alienObj);
		mEngine->AddSprite(eggObj);
		alienObj->SetVelocity(4);
		eggObj->AddAlien(alienObj);

		u->AddEgg(eggObj);
		u->DropEgg(true);
		u->EggHatch(true);

		bombObj = new CBomb();
		bombObj->SetTexture(&bombTex, 1, 6);
		bombObj->Start(0, true);
		bombObj->SetFloor(mFloor - bombObj->GetHeight());
		bombObj->MoveOffScreen();
		bombObj->SetVelocity(1);
		bombObj->SetMover(new CParaMove(bombObj->GetFloor(), 0, 1100, true, 2000));
		bombObj->mActive = false;
		mEngine->AddSprite(bombObj);


		u->AddBomb(bombObj);

		ufos[i] = u;
		aliens[i] = alienObj;
		eggs[i] = eggObj;
		bombs[i] = bombObj;
	}

	mLevelHeader = new CText();
	mLevelHeader->mView = false;
	mLevelHeader->DropShadow = true;
	mLevelHeader->SetText(mRenderer, mFont, "Attack Incoming", textYellow);
	mLevelHeader->SetPos(400 - mLevelHeader->GetWidth()/2, 200);

	mEngine->AddSprite(mLevelHeader);

	mTitle = new CAnimate();
	mTitle->SetTexture(&title, 1, 1);
	mTitle->Start(0, true);
	mTitle->SetPos(bg.GetWidth() / 2 - mTitle->GetWidth() / 2, 75);
	mEngine->AddSprite(mTitle);

	mScoreText = new CText();
	mScoreText->mView = true;
	mScoreText->SetText(mRenderer, mFont, "SCORE:", textWhite);
	mScoreText->SetPos(0, 550);
	mEngine->AddSprite(mScoreText);

	mScoreTextValue = new CText();
	mScoreTextValue->mView = true;
	mScoreTextValue->SetText(mRenderer, mFont, "0", textYellow);
	mScoreTextValue->SetPos(90, 550);
	mEngine->AddSprite(mScoreTextValue);

	mLevelText = new CText();
	mLevelText->mView = true;
	mLevelText->SetText(mRenderer, mFont, "Level:", textWhite);
	mLevelText->SetPos(700, 550);
	mEngine->AddSprite(mLevelText);

	mLevelTextValue = new CText();
	mLevelTextValue->mView = true;
	mLevelTextValue->SetText(mRenderer, mFont, "0", textYellow);
	mLevelTextValue->SetPos(775, 550);
	mEngine->AddSprite(mLevelTextValue);

	mShipsText = new CText();
	mShipsText->mView = true;
	mShipsText->SetText(mRenderer, mFont, "Jakes:", textWhite);
	mShipsText->SetPos(350, 550);
	mEngine->AddSprite(mShipsText);

	mShipsTextValue = new CText();
	mShipsTextValue->mView = true;
	mShipsTextValue->SetText(mRenderer, mFont, "0", textYellow);
	mShipsTextValue->SetPos(425, 550);
	mEngine->AddSprite(mShipsTextValue);


	mMissilesLeftBlank = new CAnimate();
	mMissilesLeftBlank->SetTexture(&progressBlankTex, 1, 1);
	mMissilesLeftBlank->Start(0, true);
	mMissilesLeftBlank->SetPos(350, 580);
	mEngine->AddSprite(mMissilesLeftBlank);

	mMissilesLeft = new CAnimate();
	mMissilesLeft->SetTexture(&progressTex, 1, 1);
	mMissilesLeft->Start(0, true);
	mMissilesLeft->SetPos(350, 580);
	mEngine->AddSprite(mMissilesLeft);

	mMissileIcon = new CAnimate();
	mMissileIcon->SetTexture(&missileTex, 1, 2);
	mMissileIcon->Stop();
	mMissileIcon->SetPos(340, 575);
	mEngine->AddSprite(mMissileIcon);

	CAnimate *explosion;
	for (i = 0; i < 20; i++)
	{
		explosion = new CAnimate();
		explosion->SetTexture(&explosionTex, 5, 16);
		explosion->AddAudio(gUfoExplosion);
		explosion->AddAudio(gUfoExplosion);
		explosion->AddAudio(gUfoExplosion);
		explosion->AddAudio(gUfoExplosion);
		explosion->AddAudio(gUfoExplosion);
		explosion->MoveOffScreen();
		explosion->Fade(true);
		mExplosions[i] = explosion;
		mEngine->AddSprite(explosion);
	}

	CAnimate *splat;
	for (i = 0; i < 5; i++)
	{
		splat = new CAnimate();
		splat->SetTexture(&splatTex, 1, 8);
		splat->AddAudio(gAlienDeath);
		splat->MoveOffScreen();
		//splat->Fade(true);
		mSplats[i] = splat;
		mEngine->AddSprite(splat);
	}

	GameOver();

	return(true);
}



void CUfoAttack::KeyUp(SDL_Keycode k)
{
	mPlayer->KeyUp(k);
}

void CUfoAttack::KeyDown(SDL_Keycode k)
{
	mPlayer->KeyDown(k);
}

//
// This should be called once eveyting frame interval to move things around. It pretty much assumes 60FPS.
//
bool CUfoAttack::Update()
{
	std::string str;
	char str2[100];
	sprintf(str2, "%d", mScore);
	str = str2;

	mScoreTextValue->SetText(mRenderer, mFont, str, textYellow);

	sprintf(str2, "%d", mLevel);
	str = str2;
	mLevelTextValue->SetText(mRenderer, mFont, str, textYellow);

	sprintf(str2, "%d", mPlayersLeft);
	str = str2;
	mShipsTextValue->SetText(mRenderer, mFont, str, textYellow);

	if (mPlayer->mNumMissilesLeft == -1)
	{
		mMissilesLeft->mXProgress = 1.0;
	}
	else
	{
		mMissilesLeft->mXProgress = (float)mPlayer->mNumMissilesLeft / (float)mPlayer->mMaxMissiles;
	}

	mEngine->Tick();

	if (mGameOver == false)
	{
		if (mReloadCounter > 0)
		{
			mReloadCounter--;
			if (mReloadCounter == 0)
			{
				mLevelHeader->mView = false;
				mReloadCounter = -1;
				PlayerUp();
				ResetAttack();
			}
		}
		Rules(); // Check for collisions and handle scoring.
	}

	if (mPlayer != NULL)
	{
		int x = mPlayer->GetPosX();
		CAlien::mRallyPointX = x + 20; // The alien rally point is where the aliens head too. Set it to the player's position.
	}

	return(true);
}