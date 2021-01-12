#include "CEgg.h"

int CEgg::mHatchTimeBase = 2000;
int CEgg::mHatchTimeRange = 1000;

CEgg::CEgg() : CAnimate()
	{
		mState = 0;
		mHatch = false;
		mEggHatch = false;
		mAlien = NULL;
		mHatchTimer.stop();

	}

	 void CEgg::EggHatch(bool b)
	{
		mEggHatch = b;
	}

	 void CEgg::Reset()
	{
		mState = 0;
		mHatch = false;
		mHatchTimer.stop();
		CAnimate::Reset();

	}

	 void CEgg::ResetAlien()
	 {
		 if (mAlien != NULL)
		 {
			 mAlien->Reset();
		 }
	 }

	 void CEgg::AddAlien(CAlien *a)
	{
		mAlien = a;
	}

	 CAlien* CEgg::GetAlien()
	 {
		 return(mAlien);
	 }

	 void CEgg::Move()
	{
		if (mMover == NULL)
			return;

		if (mActive == true)
		{
			if (mMover->GetState() == false || mYPos == mFloor)
			{
				if (mHatch == false)
				{
					mHatch = true;
					mHatchTimer.start();
					mHatchms = mHatchTimeBase + rand()%mHatchTimeRange;
				}
			}
			else
			{
				CAnimate::Move();
				if (mYPos > mFloor)
					SetPosY(mFloor);


			}
		}
	}

	 bool CEgg::Tick()
	{
		
		if (mEggHatch && mHatchTimer.isStarted())
		{

			if (mHatch == mHatchTimer.getTicks() >= mHatchms)
			{
				
				mHatchTimer.stop();
				//PlayAudio();
				mMover->SetState(false);
				Start(1, false);
				SetState(1);

				if (mAlien != NULL)
				{
					if (mAlien->GetPosX() < 0)
					{
						int x = mXPos;
						int w1, w2;

						w1 = GetWidth();
						w2 = mAlien->GetWidth();

						x = mXPos + 4;//(w1 - w2)/2;

						mAlien->SetState(0);
						mAlien->SetPosX(x);
						mAlien->mActive = false;
						mAlien->SetMoverState(true);
						mAlien->Start(0, false);
						mAlien->Stop();
					}
				}

			}
		}

		return(CAnimate::Tick());

	}

	 void CEgg::Transition()
	 {
		 if (mActive)
		 {
			 if (mState == 1)
			 {
				 mState = 0;

				 MoveOffScreen();
				 Reset();

				 if (mAlien != NULL)
				 {
					 mAlien->Start(0, false);
				 }

			 }
		 }
	 }

