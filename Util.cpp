#include "pch.h"
#include "Util.h"

Util* util;
float Util::RoundToTwoDecimals(float input)
{
	return (float)ceil(input * 100.f) / 100.f;
}