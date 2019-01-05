#ifndef __COMMON_H__
#define __COMMON_H__

#define real float
const real EPS = 0.01f;
const real FIX_TO_FLOAT_MUL = 0.0000152587890625f;
const real FLOAT_TO_FIX_MUL = 65536.0f;
//#define FLOAT_TO_FIX_MUL20  1048576.0f
const real FLOAT_TO_FIX32_MUL = 65536.0*65536.0;
const real FIX32_TO_FLOAT_MUL = 1.0f/(65536.0*65536.0);;

// rasterizer 
#define MAX_Y 1500 
// texture manager
#define TI_HEAP_SIZE 1000         
#define MAX_X 2000
const int PBUFFER_LEN = 128;
const real M_PI = (real)3.1415926;

#define RGB16(r, g, b) ((BYTE)b + ((BYTE)g<<5) + ((BYTE)r<<10))
#define RGB16RED(c) ((BYTE)((((WORD)c)>>10)&0x1F))
#define RGB16GREEN(c) ((BYTE)((((WORD)c)>>5)&0x1F))
#define RGB16BLUE(c) ((BYTE)(((WORD)c)&0x1F))

void IncMod(int &x, int mod);
void DecMod(int &x, int mod);
int round(float a);

const int XWINSIZE = 480;
const int YWINSIZE = 320;

#endif //__COMMON_H__