#pragma once

#include "CAnimate.h"
#include "CAlien.h"
#include "CTimer.h"

class CEgg :public CAnimate
{
	CAlien *mAlien;
	bool mHatch;
	bool mEggHatch;
	int mHatchms; // milliseconds to hatch;
	CTimer mHatchTimer;


public:
	static int mHatchTimeBase;
	static int mHatchTimeRange;
	CEgg();
	void EggHatch(bool b);
	void Reset();
	void ResetAlien();
	void AddAlien(CAlien *a);
	void Move();
	bool Tick();
	void Transition();
	CAlien* GetAlien();

};
