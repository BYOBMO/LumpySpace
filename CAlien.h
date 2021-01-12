#pragma once

#include "CAnimate.h"

class CAlien :public CAnimate
{

public:
	static int mRallyPointX;
	int mAbsVelocity;

	CAlien();

	void SetVelocity(int v);

	void Move();

	void Transition();

};

