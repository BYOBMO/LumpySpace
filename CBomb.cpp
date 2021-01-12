#include "CBomb.h"


CBomb::CBomb() : CAnimate()
{
}


void CBomb::Move()
{
	if (mActive == true)
	{
		if (mMover->GetState() == false || mYPos == mFloor)
		{
			Reset();
			MoveOffScreen();
		}
		else
		{
			CAnimate::Move();
			if (mYPos > mFloor)
				SetPosY(mFloor);

		}
	}
}


