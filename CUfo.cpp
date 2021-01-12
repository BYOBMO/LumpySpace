#include "CUfo.h"

#include "CRules.h"



CUfo::CUfo() : CAnimate()
{
	mNextBomb = 0;
	mNextEgg = 0;
	mDropEgg = false;
	mBombRate = 3000;
	ResetBomb();
	ResetEgg();
	mWeaponsFree = false;

	mEgg=NULL;
	mBomb = NULL;

}

void CUfo::DropEgg(bool  b)
{
	mDropEgg = b;
}

void CUfo::EggHatch(bool  b)
{
	if (mEgg != NULL)
	{
		mEgg->EggHatch(b);
	}
}

void CUfo::AddBomb(CBomb *bomb)
{
	mBomb = bomb;
}

void CUfo::AddEgg(CEgg *egg)
{
	mEgg = egg;
}

void CUfo::SetBombRate(int rate)
{
	mBombRate = rate;
}

void CUfo::Transition()
{
	if (mState == 1)
	{
		//printf("Ufo transition\n");
		mState = 0;
		Start(0, true);
	}
}

void CUfo::ResetBomb()
{
	float f = CRules::GetRandom();
	mNextBomb = (int)((float)mBombRate * f);
}

void CUfo::ResetEgg()
{
	mNextEgg = (int)((float)mBombRate * CRules::GetRandom());
}

bool CUfo::Tick()
{
	if (mActive && mWeaponsFree)
	{
		if (mBomb != NULL && !mBomb->mActive && mNextBomb == 0)
		{
			int x, y;
			y = mYPos + GetHeight();
			x = mXPos + GetWidth() / 2 - (mBomb->GetWidth()) / 2;
			mBomb->SetPos(x, y);
			mBomb->mActive = true;
			mBomb->SetMoverState(true);
			mBomb->Start(0, true);
			ResetBomb();
		}
		else if (mNextBomb > 0)
		{
			mNextBomb--;
		}

		if (mDropEgg && mEgg != NULL && !mEgg->mActive && mNextEgg == 0)
		{
			int x, y;
			y = mYPos + GetHeight();
			x = mXPos + GetWidth() / 2 - (mEgg->GetWidth()) / 2;
			mEgg->SetPos(x, y);
			mEgg->SetState(0);
			mEgg->mActive = true;
			mEgg->SetMoverState(true);
			mEgg->Start(0, false);
			ResetEgg();
		}
		else if (mNextEgg > 0)
		{
			mNextEgg--;
		}
	}

	return(CAnimate::Tick());
}

void CUfo::SetVelocity(int v)
{
	CAnimate::SetVelocity(v);
}