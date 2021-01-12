#include "CMissile.h"



CMissile::CMissile() : CAnimate()
{

}

void CMissile::Move()
{
	int x1, y1;

	if (mActive == true && mMover->GetState() == false)
	{
		mActive = false;
		mState = 0;
		MoveOffScreen();
	}
	else if (mActive)
	{
		x1 = mXPos;
		y1 = mYPos;

		CAnimate::Move();

		if (y1 > mYPos)
		{
			mState = 0;
			SetFrame(0);
		}
		else
		{
			mState = 1;
			SetFrame(1);
		}
	}
}

