#include "CBoxBounce.h"





CBoxBounce::CBoxBounce(int x1, int y1, int x2, int y2) :CMotion()
	{
		mX1 = x1;
		mY1 = y1;
		mX2 = x2;
		mY2 = y2;
		mVelocity = 1;
		mAbsX = mXVel = mVelocity;
		mAbsY = mYVel = mVelocity;
		mXv = 1;
	}

 void CBoxBounce::Welcome(int x, int y, int w, int h, int v)
	{
		mX = x;
		mY = y;
		mW = w;
		mH = h;
		mVelocity = v;
		mXVel = mVelocity;
		mYVel = mVelocity;
		//mDir = mDir;
	}

 void CBoxBounce::Tick()
	{
		int x, y;

		if (mState)
		{
			x = mX + mXVel;
			if (x > mX2 - mW || x<0)
			{
				mXVel = mXVel * -1;
				mXv = mXv * -1;
			}
			else
			{
				mX = x;
			}

			y = mY + mYVel;
			if (y > mY2 - mH || y<0)
			{
				mYVel = mYVel * -1;
			}
			else
			{
				mY = y;
			}


		}
	}


 void CBoxBounce::SetPos(int x, int y)
 {
	 CMotion::SetPos(x, y);
	 mX = x;
	 mY = y;
 }

 void CBoxBounce::SetVelocity(int v)
 {
	 CMotion::SetVelocity(v);
	 mVelocity = v;
	 mXVel = mVelocity;
	 mYVel = mVelocity;
  }