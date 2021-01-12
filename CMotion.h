#pragma once
class CMotion
{

protected:
	int mX, mY;
	int mW, mH;
	bool mState;
	int mVelocity;

public:
	virtual void Welcome(int x, int y, int w, int h, int v)=0;
	virtual void Tick()=0;
	virtual void Init() {};

	CMotion();
	virtual void SetPos(int x, int y);
	virtual void SetVelocity(int v);
	int GetX();
	int GetY();
	bool GetState();
	virtual void SetState(bool s);
	virtual void Reset() {}
};

