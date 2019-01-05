#include "stdafx.h"
#include "common.h"

//////////////////////////////////////////////////////////////////////

int round(float s)
{
    int l = (int)s;
    if (s - l > 0.5) return l + 1;
    return l;
}

//////////////////////////////////////////////////////////////////////

void IncMod(int &x, int mod)
{
    x = (x + 1)%mod;
}

//////////////////////////////////////////////////////////////////////

void DecMod(int &x, int mod)
{
    if (x == 0) x = mod - 1;
    else x--;
}

//////////////////////////////////////////////////////////////////////

