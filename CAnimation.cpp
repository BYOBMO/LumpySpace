#include "CAnimation.h"



CAnimation::CAnimation() {
	int i;

	mTexture = NULL;
	CurrentFrame = 0;
	for (i = 0; i < 10; i++)
	{
		MaxFrames[i] = 0;
	}
	MaxRows = 1;
	FrameInc = 1;
	Repeat = true;
	Enabled = true;
	mState = 0;
	FrameRate = 50; //Milliseconds
	OldTime = 0;
	Velocity = 1;

	Oscillate = false;
	X = 0, Y = 0;
	Row = 0;
	mMover = NULL;
}

 void CAnimation::MoveOffScreen()
{
	X = (-100 - W);

	Enable(false);
}

void CAnimation::OnAnimate()
{
	if (OldTime + FrameRate > SDL_GetTicks())
	{
		return; // Only switch frames after timer has expired.
	}

	OldTime = SDL_GetTicks();


	if (Enabled)
	{
		CurrentFrame += FrameInc;

		if (Oscillate)
		{
			// Oscillate reverses the animation at the end.
			if (FrameInc > 0)
			{
				if (CurrentFrame >= MaxFrames[Row])
				{
					// Past the end. Reverse direction.
					CurrentFrame = MaxFrames[Row] -1;
					FrameInc = -FrameInc;
				}
			}
			else
			{
				//Past the beginning. reverse direciton.
				if (CurrentFrame <= 0)
				{
					CurrentFrame = 0;
					FrameInc = -FrameInc;
					Enabled = Repeat; // Keep oscillating of repeat is turned on.
				}
			}
		}
		else
		{
			if (CurrentFrame >= MaxFrames[Row])
			{
				CurrentFrame = 0;
				Enabled = Repeat; // Keep animating if repeat is turned on.
				if (Enabled == false)
				{
					Transition(); // Call teh sub-class's Transition method to find out what to do at the end of the animation.
				}
			}
		}
	}
}

void CAnimation::Enable(bool enable)
{
	Enabled = enable;
	if (enable)
	{
		if (mMover != NULL)
		{
			mMover->Reset();
		}
		CurrentFrame = 0;
		OldTime = SDL_GetTicks();
	}
}


void CAnimation::SetTexture(CTexture *tex, int rows, int frames)
{
	int i;
	MaxRows = rows;
	for (i = 0; i < 10; i++)
	{
		MaxFrames[i] = frames;
	}

	mTexture = tex;
	W = mTexture->GetWidth() / frames;
	H = mTexture->GetHeight() / rows;
}

void CAnimation::SetFrameRate(int Rate) 
{
	FrameRate = Rate;
}

void CAnimation::SetCurrentFrame(int Frame)
{
	if (Frame < 0 || Frame >= MaxFrames[Row]) return;

	CurrentFrame = Frame;
}

void CAnimation::SetCurrentRow(int row)
{
	if (row < 0 || row >= MaxRows) return;

	Row = row;
}

int CAnimation::GetCurrentFrame() {
	return CurrentFrame;
}

void CAnimation::Render(SDL_Renderer *renderer)
{
	// Render to screen.
	if (mTexture != NULL)
	{
		SDL_Rect src;

		int curFrame = GetCurrentFrame();

		// Calculate which part of the texture is the frame we want to draw.
		src = { CurrentFrame*W, Row*H, W, H };


		SDL_Rect dst = { X, Y, W, H };

		mTexture->Draw(renderer, &src, &dst, 0.0);
	}
}

void CAnimation::SetMover(CMotion *mover)
{
	mMover = mover;
	mMover->Welcome(X, Y, W, H, Velocity);
}

void CAnimation::Move()
{
	if (mMover != NULL)
	{
		mMover->Tick();
		X = mMover->GetX();
		Y = mMover->GetY();
	}
}

void CAnimation::SetMoverState(bool state)
{
	if (mMover != NULL)
	{
		mMover->SetState(state);
	}
}

void CAnimation::Reset()
{
	SetCurrentFrame(0);
	mState = 0;
	MoveOffScreen();
	if (mMover != NULL)
		mMover->Init();

	Enabled = false;
}

void CAnimation::SetPosition(int x, int y)
{
	X = x;
	Y = y;

	if (mMover != NULL)
	{
		mMover->SetPos(x, y);
	}
}
void CAnimation::SetVelocity(int v)
{
	Velocity = v;
	if (mMover != NULL)
	{
		mMover->SetVelocity(v);
	}
}

void CAnimation::SetState(bool s)
{
	mState = s;
	if (mMover != NULL)
	{
		mMover->SetState(s);
	}
}

void CAnimation::SetBounds(CRectangle bounds)
{
	// Set the hit box bounds for the sprite.
	mBounds = bounds;
}

void CAnimation::SetBounds(int x, int y, int w, int h)
{
	// Set the hit box bounds for the sprite.
	mBounds.x = x;
	mBounds.y = y;
	mBounds.w = w;
	mBounds.h = h;

}