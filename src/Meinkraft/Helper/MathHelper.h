#pragma once

class MathHelper
{
public:
	
	static bool between(int number, int lower, int upper)
	{
		return (unsigned)(number-lower) <= (upper-lower);
	}
};
