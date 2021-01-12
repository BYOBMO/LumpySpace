#pragma once
#include "CAnimate.h"
#include "CEgg.h"
#include "CBomb.h"

class CUfo :public CAnimate
{
	CEgg *mEgg;
	CBomb *mBomb;
	int mNextBomb;
	int mNextEgg;
	int mBombRate;
	bool mDropEgg;

public:
	bool mWeaponsFree;

	CUfo();
	void DropEgg(bool  b);
	void EggHatch(bool  b);
	void AddBomb(CBomb *bomb);
	void AddEgg(CEgg *egg);
	void SetBombRate(int rate);
	void Transition();
	void ResetBomb();
	void ResetEgg();
	bool Tick();
	void SetVelocity(int v);
};

