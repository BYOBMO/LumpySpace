#pragma once
#include "CAnimate.h"
#include "CMissile.h"
#include "CBullet.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class CPlayer : public CAnimate
{
	CMissile *mMissiles[10];
	int mNumMissiles;
	bool mMissileLoaded;

	CBullet *mBullets[10];
	int mNumBullets;
	bool mBulletLoaded;
	bool mLeft, mRight;

	
public:
	CPlayer();
	void Move();
	void AddMissile(CMissile *m);
	void AddBullet(CBullet *b);
	void LaunchBullet(int dir);
	void KeyDown(SDL_Keycode k);
	void Reset();
	void KeyUp(SDL_Keycode k);
	void KeyPress(SDL_Keycode k);
	void Transition();
	int mNumMissilesLeft;
	int mMaxMissiles;
	void Start(int anim, bool loop);
};


