#include "CPlayer.h"
#include <stdio.h>
#include <sstream>

CPlayer::CPlayer() : CAnimate()
{
	int i;


	for (i = 0; i < 10; i++)
	{
		mMissiles[i] = NULL;
		mBullets[i] = NULL;
	}


	mNumMissiles = 0;
	mNumBullets = 0;
	mMissileLoaded = true;
	mLeft = false;
	mRight = false;
	mNumMissilesLeft = 0;
	mMaxMissiles = 10;
}

void CPlayer::Move()
{
	if (mState == 0)
	{
		if (mRight == true)
			SetVelocity(7);
		else if (mLeft == true)
			SetVelocity(-7);
		else
			SetVelocity(0);

		CAnimate::Move();
	}
}

void CPlayer::Start(int anim, bool loop)
{
	CAnimate::Start(anim, loop);

}

void CPlayer::AddMissile(CMissile *m)
{
	if (mNumMissiles < 10)
	{
		mMissiles[mNumMissiles] = m;
		mNumMissiles++;
	}
}

void CPlayer::AddBullet(CBullet *b)
{
	if (mNumBullets < 5)
	{
		mBullets[mNumBullets] = b;
		mNumBullets++;
	}
}

	 void CPlayer::LaunchBullet(int dir)
	{
		int i;
		if (mMissileLoaded && mActive == true)
		{
			for (i = 0; i<mNumBullets; i++)
			{
				if (mBullets[i]->mActive == false)
				{
					if (dir < 0)
					{
						mBullets[i]->SetPosX(mXPos);
					}
					else
					{
						mBullets[i]->SetPosX(mXPos + GetWidth());
					}

					//mBullets[i].SetPosY(340);
					mBullets[i]->SetMoverState(true);
					mBullets[i]->Start(0, true);
					mBullets[i]->SetVelocity(dir * abs(mBullets[i]->GetVelocity()));
					mBullets[i]->mActive = true;
					mBullets[i]->SetState(0);
					//mBullets[i]->PlayAudio();
					mMissileLoaded = false;
					break;
				}
			}
		}
	}

	 void CPlayer::KeyDown(SDL_Keycode k)
	{
		int i;

		if (k == SDLK_a)
			mLeft = true;//SetVelocity(-7);
		else if (k == SDLK_d)
			mRight = true;//SetVelocity(7);
		else if (k == SDLK_UP)
		{
			if (mMissileLoaded && mActive == true)
			{
				for (i = 0; i<mNumMissiles; i++)
				{
					if (mMissiles[i]->mActive == false && (mNumMissilesLeft>0 || mNumMissilesLeft==-1))
					{
						if (mNumMissilesLeft > 0)
						{
							mNumMissilesLeft--;
						}
						mMissiles[i]->SetPosX(mXPos + GetWidth() / 2 - (mMissiles[i]->GetWidth()) / 2);
						mMissiles[i]->SetPosY(1000);
						mMissiles[i]->SetMoverState(true);
						mMissiles[i]->mActive = true;
						//mMissiles[i]->PlayAudio();
						mMissileLoaded = false;
						Start(0, false);
						
						break;
					}
				}
			}
		}
		else if (k == SDLK_LEFT)
		{
			if (mMissileLoaded)
			{
				Start(1, false);
				LaunchBullet(-1);
				
			}
		}
		else if (k == SDLK_RIGHT)
		{
			if (mMissileLoaded)
			{
				Start(2, false);
				LaunchBullet(1);
				
			}
		}
	}

	 void CPlayer::Reset()
	{
		CAnimate::Reset();
		mLeft = false;
		mRight = false;
	}

	 void CPlayer::KeyUp(SDL_Keycode k)
	{
		if (k == SDLK_a)
			mLeft = false;
		else if (k == SDLK_d)
			mRight = false;
		else if (k == SDLK_UP || k == SDLK_LEFT || k == SDLK_RIGHT)
			mMissileLoaded = true;


	}

	 void CPlayer::KeyPress(int k)
	{
	}

	 void CPlayer::Transition()
	 {
		 if (mState == 1)
		 {
			 Reset();
		 }
	 }



