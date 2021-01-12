#pragma once
#include <SDL2/SDL.h>
#include "CTexture.h"
#include "CMotion.h"

class CAnimation 
{
	private:
		int    CurrentFrame;
		int     FrameInc;
		int     FrameRate; //Milliseconds
		long    OldTime;
		CRectangle mBounds;

	protected:
		CMotion *mMover;
		int mFloor;

	public:
		int    MaxFrames[10];
		int    MaxRows;
		bool    Oscillate;
		CTexture *mTexture;
		int X, Y;
		int W, H;
		int Velocity;
		int Row;
		bool Enabled;
		bool Repeat;
		bool mState;

	public:
		CAnimation();
		virtual void OnAnimate();
		void Enable(bool enable);
		void SetFrameRate(int Rate);
		void SetCurrentFrame(int Frame);
		int GetCurrentFrame();
		void Render(SDL_Renderer *renderer);
		void SetTexture(CTexture *tex, int rows, int frames);
		void SetCurrentRow(int row);
		void SetMover(CMotion *mover);
		void MoveOffScreen();
		virtual void Move();
		virtual void SetMoverState(bool state);
		virtual void SetPosition(int x, int y);
		virtual void SetVelocity(int v);
		virtual void SetState(bool s);
		virtual void Reset();
		void SetBounds(CRectangle bounds);
		void SetBounds(int x, int y, int w, int h);
		virtual void Transition() {}
};


