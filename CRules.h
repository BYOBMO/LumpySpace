#pragma once

#include "CTexture.h"

class CRules
{
	
public:
	static void Init()
	{
		
	}

	static float GetRandom()
	{
		int i = rand() % 100;

		float f = (float)i / 100.0f;
		return(f);
	}
};

