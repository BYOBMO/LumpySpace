#include "CMotion.h"

 CMotion::CMotion()
{
	mX = 0;
	mY = 0;
	mW = 0;
	mH = 0;
	mState = true;
	mVelocity = 0;
}

void CMotion::SetPos(int x, int y)
{
	mX = x;
	mY = y;
}

void CMotion::SetVelocity(int v)
{
	mVelocity = v;
}

int CMotion::GetX()
{
	return(mX);
}

int CMotion::GetY()
{
	return(mY);
}

bool CMotion::GetState()
{
	return(mState);
}

void CMotion::SetState(bool s)
{
	mState = s;
}


