#pragma once

#include "CMotion.h"

class CXMove : public CMotion
{
	int mLeft, mRight;
	int mBoundHit;

public:
	CXMove(int left, int right, int boundHit);
	void Welcome(int x, int y, int w, int h, int v);
	void Tick();


};

