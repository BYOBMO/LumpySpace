#include "CAlien.h"



int CAlien::mRallyPointX;



CAlien::CAlien() :CAnimate()
{
	mAbsVelocity = 0;
}

	 void CAlien::SetVelocity(int v)
	{
		mAbsVelocity = abs(v);
		CAnimate::SetVelocity(v);
	}

	 void CAlien::Move()
	{
		if (mActive)
		{
			if (mState == 1)
			{
				if (mXPos<=mRallyPointX && mVelocity<0)
				{
					SetVelocity(mAbsVelocity);
					Start(2, true);
				}
				else if (mXPos>=mRallyPointX && mVelocity>0)
				{
					Start(1, true);
					SetVelocity(-1 * mAbsVelocity);
				}
				CAnimate::Move();
			}

		}

	}

	 void CAlien::Transition()
	{
		if (mState == 0)
		{
			SetMoverState(true);
			mState = 1;
			mActive = true;
			if (mXPos<mRallyPointX && mVelocity>0)
			{
				SetVelocity(-1 * mAbsVelocity);
				Start(1, true);
			}
			else if (mXPos>mRallyPointX && mVelocity<0)
			{
				Start(2, true);
				SetVelocity(mAbsVelocity);
			}
		}
		else if (mState == 1)
		{
		}
		else if (mState == 2)
		{
			Reset();
			MoveOffScreen();
		}
	}

