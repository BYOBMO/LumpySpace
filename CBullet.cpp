#include "CBullet.h"


CBullet::CBullet() : CAnimate()
{
		
}


void CBullet::Move()
{
	if (mActive == true)
	{
		if (mMover->GetState() == false)
		{
			Reset();
			MoveOffScreen();
		}
		else
		{
			CAnimate::Move();
		}
	}
}



