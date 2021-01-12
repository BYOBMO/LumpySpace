#include "CXMove.h"




CXMove::CXMove(int left, int right, int boundHit) : CMotion()
{
	mLeft = left;
	mRight = right;
	mBoundHit = boundHit;
}

void CXMove::Welcome(int x, int y, int w, int h, int v)
{
	mX = x;
	mY = y;
	mW = w;
	mH = h;
	mVelocity = v;
}

void CXMove::Tick()
{
	if (mState)
	{
		mX = mX + mVelocity;
		if (mX > mRight - mW)
		{
			if (mBoundHit == 1)
				mX = mRight - mW;
			else if (mBoundHit == 2)
			{
				mVelocity = mVelocity*-1;
			}
			else if (mBoundHit == 3)
			{
				mState = false;
			}

		}
		else if (mX < mLeft)
		{
			if (mBoundHit == 1)
				mX = mLeft;
			else if (mBoundHit == 2)
			{
				mVelocity = mVelocity*-1;
			}
			else if (mBoundHit == 3)
			{
				mState = false;
			}
		}
	}
}


