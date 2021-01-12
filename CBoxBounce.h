#pragma once
#include "CMotion.h"


class CBoxBounce : public CMotion
{
	int mX1, mY1, mX2, mY2;
	int mXVel, mYVel;
	int mAbsX, mAbsY;
	int mXv;

	//static int mDir = 1;

public:
	CBoxBounce(int x1, int y1, int x2, int y2);
	void Welcome(int x, int y, int w, int h, int v);
	void Tick();
	virtual void SetPos(int x, int y);
	virtual void SetVelocity(int v);
};


