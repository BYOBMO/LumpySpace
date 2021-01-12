#pragma once
#include "CMotion.h"

class CParaMove: public CMotion
{
	int *mPoints;
	int mIndex;
	int mFloor;
	int mNPoints;

	bool mOffset;
	int mStartY;
	int mStart;

public:
	CParaMove(int floor, int top, int start, bool offset, int n);
	void Init();
	void SetPos(int x, int y);
	void Welcome(int x, int y, int w, int h, int v);
	void Tick();
	void Reset();
};

