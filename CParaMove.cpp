#include "CParaMove.h"
#include <stdio.h>      /* printf */
#include <math.h>


CParaMove::CParaMove(int floor, int top, int start, bool offset, int n) : CMotion()
{

	double delta, f, range;
	int i;
	int mid = n / 2;

	mFloor = floor;
	mNPoints = n;
	mPoints = new int[n];
	range = sqrt(floor - top + 1);

	delta = range / mid;

	f = -range;
	for (i = 0; i< mid; i++)
	{
		mPoints[i] = top + (int)(f * f);
		mPoints[n - 1 - i] = top + (int)(f * f);
		f = f + delta;
	}

	mVelocity = 1;
	mStart = start;
	mIndex = start;
	mOffset = offset;
}

void CParaMove::Reset()
{
	mIndex = mStart;
}

	void CParaMove::Init()
{
	mIndex = mStart;
}

void CParaMove::SetPos(int x, int y)
{
	CMotion::SetPos(x, y);
	mStartY = y;
}

void CParaMove::Welcome(int x, int y, int w, int h, int v)
{
	mX = x;
	mY = y;
	mStartY = y;
	mW = w;
	mH = h;
	mVelocity = v;
}

void CParaMove::Tick()
{
	int y;

	if (mState)
	{
		y = mPoints[mIndex];


		mIndex = mIndex + mVelocity;
		if (mIndex >= mNPoints)
		{
			mIndex = 0;
			mState = false;
			if (mOffset == false)
				y = mFloor;
		}

		if (mIndex + mVelocity >= mNPoints)
			mIndex = mNPoints - mVelocity;

		if (mOffset == true)
			mY = mStartY + y;
		else
			mY = y;

	}
}

